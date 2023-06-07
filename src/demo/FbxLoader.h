#pragma once
#include <iostream>
#include <fbxsdk.h>
#include "VaoContainer.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glew.h>

class FbxLoader
{
private:
	FbxManager* manager = NULL;
	FbxImporter* importer = NULL;
	FbxIOSettings* settings = NULL;
	FbxScene* scene = NULL;

	std::vector<GLfloat> vertices;
	std::vector<GLubyte> texture_data;

	void RecursiveSearchForMeshStores(FbxNode* node, VaoContainer* container);
	/*
		
	*/
	void MeshAndUVStore(FbxMesh* mesh, VaoContainer* container);
	/*
	
	*/
	void MeshStore(FbxMesh* mesh, VaoContainer* container);
public:
	FbxLoader();
	~FbxLoader();
	/*
		file		- path to a fbx file - not implemented yet
		container	- pointer to a container object which contains the VAOs for each mesh in scene
	*/
	void Load(const char* file, VaoContainer* container);
	void LoadTexture(const char* path);
};
