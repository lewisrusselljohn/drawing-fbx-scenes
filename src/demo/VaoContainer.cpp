#include "VaoContainer.h"

VaoContainer::VaoContainer()
{

}

VaoContainer::~VaoContainer()
{
	for (size_t i = 0; i < vaos.size(); i++)
	{
		GLuint vao = vaos[i];
		GLuint vbo = vbos[i];

		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}

	vbo_sizes.clear();
	vbos.clear();
	vaos.clear();
}

void VaoContainer::StoreMeshAndUV(std::vector<GLfloat>* vertices, 
	std::vector<GLubyte>* texture_data, glm::mat4 world)
{
	GLuint vao, vbo, vbo_size, texture_id, texture_data_size;
	GLfloat* vertex_data = NULL;
	GLubyte* raw_texture_data = NULL;
	if (vertices)
	{
		// Dereference to obtain a reference to vector? confusing
		std::vector<GLfloat>& v = *vertices;
		std::vector<GLubyte>& t = *texture_data;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// 
		GLubyte pixel[3] = { 0, 0, 255 };
		if(t.size() == 0)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
		else
		{
			texture_data_size = t.size();
			raw_texture_data = t.data();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB,
				GL_UNSIGNED_BYTE, raw_texture_data);
		}
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		vbo_size = v.size();
		vertex_data = v.data();

		glBufferData(GL_ARRAY_BUFFER, vbo_size * sizeof(GLfloat), vertex_data, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			5 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(2);

		vaos.push_back(vao);
		vbos.push_back(vbo);
		vbo_sizes.push_back(vbo_size);
		mats.push_back(world);
	}
}

void VaoContainer::DrawVaos(GLuint programid)
{
	for (size_t i = 0; i < vaos.size(); i++)
	{
		GLuint vao = vaos[i];
		GLuint vbo_size = vbo_sizes[i];
		glm::mat4 world = mats[i];

		GLuint worldLoc = glGetUniformLocation(programid, "world");
		glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(world));
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, vbo_size);
	}
}
