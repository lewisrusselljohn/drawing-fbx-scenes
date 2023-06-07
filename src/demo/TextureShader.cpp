#include "TextureShader.h"

TextureShader::TextureShader(const char* vs_source, const char* fs_source)
{
	mvLoc = projLoc = -1;

	program = glCreateProgram();

	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &vs_source, NULL);
	glCompileShader(vert_shader);

	// Check for vertex shader compilation errors
	int success;
	char infoLog[512];
	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vert_shader, 512, NULL, infoLog);
		std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
	}

	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &fs_source, NULL);
	glCompileShader(frag_shader);

	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(frag_shader, 512, NULL, infoLog);
		std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
	}

	// Attach the shaders to the program
	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);

	// Link the shader program
	glLinkProgram(program);

	// Check for shader program linking errors
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "Shader program linking failed: " << infoLog << std::endl;
	}

	// Delete the shaders as they are now linked to the program
	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	mvLoc = glGetUniformLocation(program, "mv");
	projLoc = glGetUniformLocation(program, "proj");
	worldLoc = glGetUniformLocation(program, "world");
}

TextureShader::~TextureShader()
{

}

void TextureShader::UseProgram(glm::mat4 proj, glm::mat4 mv)
{
	glUseProgram(program);
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
}
