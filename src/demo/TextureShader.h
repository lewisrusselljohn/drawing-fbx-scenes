#pragma once
#include <glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class TextureShader
{
private:
	GLuint program, vert_shader, frag_shader;
	GLint mvLoc, projLoc, worldLoc;
public:
	TextureShader(const char* vertshader, const char * fragshader);
	~TextureShader();
	void UseProgram(glm::mat4 proj, glm::mat4 mv);

	GLuint GetProgramId() { return program; }
};
