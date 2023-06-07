#include "Cage.h"

Cage::Cage(double size, int increment)
{
	std::vector<GLfloat> lines;
	double half = size / 2.0;
	double x0 = -half;
	double y0 = -half;
	double z0 = -half;
	double jump = size / (double)increment;

	for (int i = 0; i <= increment; i++)
	{
		for (int j = 0; j <= increment; j++)
		{
			for (int k = 0; k <= increment; k++)
			{
				double x1 = x0 + jump * i;
				double y1 = y0 + jump * j;
				double z1 = z0 + jump * k;

				double x2 = x0 + jump * (i + 1);
				double y2 = y0 + jump * (j + 1);
				double z2 = z0 + jump * (k + 1);

				// Bottom
				lines.push_back(x1);
				lines.push_back(y1);
				lines.push_back(z0);

				lines.push_back(x0);
				lines.push_back(y1);
				lines.push_back(z0);

				lines.push_back(x1);
				lines.push_back(y1);
				lines.push_back(z0);

				lines.push_back(x1);
				lines.push_back(y0);
				lines.push_back(z0);

				// Top
				lines.push_back(x1);
				lines.push_back(y1);
				lines.push_back(z0 + size);

				lines.push_back(x0);
				lines.push_back(y1);
				lines.push_back(z0 + size);

				lines.push_back(x1);
				lines.push_back(y1);
				lines.push_back(z0 + size);

				lines.push_back(x1);
				lines.push_back(y0);
				lines.push_back(z0 + size);

				// If to prevent a set of lines bleeding out the side of the box
				if(j != increment && k != increment)
				{
					// Back
					lines.push_back(x0);
					lines.push_back(y1);
					lines.push_back(z1);

					lines.push_back(x0);
					lines.push_back(y2);
					lines.push_back(z1);

					lines.push_back(x0);
					lines.push_back(y1);
					lines.push_back(z1);

					lines.push_back(x0);
					lines.push_back(y1);
					lines.push_back(z2);

					// Forward
					lines.push_back(x0 + size);
					lines.push_back(y1);
					lines.push_back(z1);

					lines.push_back(x0 + size);
					lines.push_back(y2);
					lines.push_back(z1);

					lines.push_back(x0 + size);
					lines.push_back(y1);
					lines.push_back(z1);

					lines.push_back(x0 + size);
					lines.push_back(y1);
					lines.push_back(z2);
				}

				if(i != increment && k != increment)
				{
					// right
					lines.push_back(x1);
					lines.push_back(y0);
					lines.push_back(z1);

					lines.push_back(x2);
					lines.push_back(y0);
					lines.push_back(z1);

					lines.push_back(x1);
					lines.push_back(y0);
					lines.push_back(z1);

					lines.push_back(x1);
					lines.push_back(y0);
					lines.push_back(z2);

					// left
					lines.push_back(x1);
					lines.push_back(y0 + size);
					lines.push_back(z1);

					lines.push_back(x2);
					lines.push_back(y0 + size);
					lines.push_back(z1);

					lines.push_back(x1);
					lines.push_back(y0 + size);
					lines.push_back(z1);

					lines.push_back(x1);
					lines.push_back(y0 + size);
					lines.push_back(z2);
				}
				else
				{
					if(k != increment)
					{
						// Add lines for the final corner
						lines.push_back(x0 + size);
						lines.push_back(y0 + size);
						lines.push_back(z1);

						lines.push_back(x0 + size);
						lines.push_back(y0 + size);
						lines.push_back(z2);
					}
				}
			}
		}
	}

	// VAO, VBO setup

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	GLubyte pixel[3] = { 0, 0, 255 };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	vbo_size = lines.size();
	GLfloat* vertex_data = lines.data();
	glBufferData(GL_ARRAY_BUFFER, vbo_size * sizeof(GLfloat), vertex_data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	lines.clear();
}

Cage::~Cage()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Cage::DrawLines()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, vbo_size);
	glBindVertexArray(0);
}

void Cage::DrawLines(GLuint programid)
{
	glm::mat4 world = glm::mat4(1.0f);
	GLuint worldLoc = glGetUniformLocation(programid, "world");
	glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(world));
	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, vbo_size);
	glBindVertexArray(0);
}
