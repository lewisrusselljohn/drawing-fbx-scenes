#pragma once
#include <glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Cage
{
private:
	GLuint vao, vbo, vbo_size, texture_id;
public:
	Cage(double size, int increment);
	~Cage();

	void DrawLines();
	void DrawLines(GLuint);
};
