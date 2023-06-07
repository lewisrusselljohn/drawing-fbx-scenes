#pragma once
#include <vector>
#include <glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "TextureShader.h"
#include "Camera.h"

class VaoContainer
{
private:
	std::vector<GLuint> vaos;
	std::vector<GLuint> vbos;
	std::vector<GLuint> vbo_sizes;
	std::vector<GLuint> texture_ids;
	std::vector<glm::mat4> mats;
public:
	VaoContainer();
	~VaoContainer();
	/*
		from vertices create associate vertex array object and 
		vertex buffer object and stores the vertices in the GPU for drawing.
	*/
	void StoreMeshAndUV(std::vector<GLfloat>* vertices, std::vector<GLubyte>* texture_data, glm::mat4 world);
	void DrawVaos(GLuint program_id);
};
