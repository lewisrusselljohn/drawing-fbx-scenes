#include "FbxLoader.h"

FbxLoader::FbxLoader()
{
	manager = FbxManager::Create();
	settings = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(settings);
	importer = FbxImporter::Create(manager, "importer");
}

FbxLoader::~FbxLoader()
{
	settings->Destroy();
	importer->Destroy();
	manager->Destroy();
}

/*
	file		- path to a fbx file
	container	- pointer to a VaoContainer object which contains the VAOs 
				  for each mesh in scene
*/
void FbxLoader::Load(const char* file, VaoContainer* container)
{
	bool import_success = importer->Initialize(file, -1, manager->GetIOSettings());

	if (!import_success)
	{
		FbxString error = importer->GetStatus().GetErrorString();
		std::cout << "Import error: " << error.Buffer() << std::endl;
		return;
	}

	scene = FbxScene::Create(manager, "scene");
	importer->Import(scene);
	
	FbxNode* root = scene->GetRootNode();

	RecursiveSearchForMeshStores(root, container);
}

void FbxLoader::RecursiveSearchForMeshStores(FbxNode* node, VaoContainer* container)
{
	int child_count = node->GetChildCount();
	FbxNode* child = NULL;
	FbxNodeAttribute* attribute = NULL;
	FbxMesh* mesh = NULL;
	
	int poly_count = 0;
	int* poly_vertices = NULL;
	for (int i = 0; i < child_count; i++)
	{

		child = node->GetChild(i);
		attribute = child->GetNodeAttribute();

		if (child && attribute)
		{
			if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
			{
				/*
					Identified mesh - push vertices into vert vector after clearing
					 it and call container store method
				*/
				mesh = child->GetMesh();
				if (mesh)
				{
					// Only doing triangle meshes atm
					if (mesh->IsTriangleMesh())
					{
						
						MeshAndUVStore(mesh, container);
					}
				}
			}
		}
		RecursiveSearchForMeshStores(child, container);
	}

}

/*
	fbx sdk uv sample loader modified here.
	Really inefficient at the moment (for everyuv of the mesh, storing an entire mesh)
	But it's just to get something working for now.
*/
void FbxLoader::MeshAndUVStore(FbxMesh* mesh, VaoContainer* container)
{
	vertices.clear();
	
	// Get the world matrix for the mesh
	FbxAMatrix global = mesh->GetNode()->EvaluateGlobalTransform();
	glm::mat4 world = glm::mat4();
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			world[i][j] = global.Get(i, j);
		}
	}

	FbxStringList uvSetNames;
	mesh->GetUVSetNames(uvSetNames);

	for (int set_index = 0; set_index < uvSetNames.GetCount(); set_index++)
	{
		const char* name = uvSetNames.GetStringAt(set_index);
		const FbxGeometryElementUV* uvElement = mesh->GetElementUV(name);
		//printf("Name: %s \n", name);

		if (!uvElement)
			continue;

		LoadTexture(name);

		// only support mapping mode eByPolygonVertex and eByControlPoint
		if (uvElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
			uvElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
			return;

		//index array, where holds the index referenced to the uv data
		const bool useIndex = uvElement->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int indexCount = (useIndex) ? uvElement->GetIndexArray().GetCount() : 0;

		//iterating through the data by polygon
		const int polyCount = mesh->GetPolygonCount();

		if (uvElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			int polyIndexCounter = 0;
			for (int polyIndex = 0; polyIndex < polyCount; ++polyIndex)
			{
				const int polySize = mesh->GetPolygonSize(polyIndex);
				for(int vertexIndex = 0; vertexIndex < polySize; ++vertexIndex)
				{ 
					if (polyIndexCounter < indexCount)
					{
						FbxVector2 uvValue;

						//the UV index depends on the reference mode
						int uvIndex = useIndex ? uvElement->GetIndexArray().GetAt(polyIndexCounter) : polyIndexCounter;
						
						uvValue = uvElement->GetDirectArray().GetAt(uvIndex);
						polyIndexCounter++;
					}
				}
			}
		}
		else if (uvElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int polyIndexCounter = 0;
			for (int polyIndex = 0; polyIndex < polyCount; ++polyIndex)
			{
				const int polySize = mesh->GetPolygonSize(polyIndex);
				for (int vertexIndex = 0; vertexIndex < polySize; ++vertexIndex)
				{
					if (polyIndexCounter < indexCount)
					{
						FbxVector2 uvValue;
						FbxVector4 controlPoint;
						int pointIndex = mesh->GetPolygonVertex(polyIndex, vertexIndex);
						// Get control point at pointIndex
						controlPoint = mesh->GetControlPointAt(pointIndex);

						//the UV index depends on the reference mode
						int lUVIndex = useIndex ? uvElement->GetIndexArray().GetAt(polyIndexCounter) : polyIndexCounter;

						uvValue = uvElement->GetDirectArray().GetAt(lUVIndex);

						vertices.push_back(controlPoint[0]);
						vertices.push_back(controlPoint[1]);
						vertices.push_back(controlPoint[2]);
						vertices.push_back(uvValue[0]);
						vertices.push_back(uvValue[1]);

						polyIndexCounter++;
					}
				}
			}
		}
	}

	container->StoreMeshAndUV(&vertices, &texture_data, world);
}

void FbxLoader::LoadTexture(const char* path)
{
	texture_data.clear();
	FILE* file = NULL;
	file = fopen("g128.data", "rb");

	int c, file_size, i;

	if (file)
	{
		fseek(file, 0, SEEK_END);
		file_size = ftell(file);
		//tex_data = new GLubyte[file_size - 1];
		rewind(file);
		for (i = 0; i < file_size; i++)
		{
			c = fgetc(file);
			//tex_data[i] = c;
			texture_data.push_back(c);
		}

		fclose(file);
	}
}
