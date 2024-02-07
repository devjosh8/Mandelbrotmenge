#pragma once

#include <glad/glad.h>
#include <string>

struct Shader {

	Shader(const char* vertexShaderFilename, const char* fragmentShaderFilename);
	virtual ~Shader();

	void bind();
	void unbind();
	void init();

	GLint getUniformLocation(const GLchar* name);


private:
	GLuint compile(std::string shaderSource, GLenum shaderType);

	std::string parse(const char* filename);

	GLuint createShader(const char* vertexShaderFilename, const char* fragmentShaderFilename);

	GLuint shaderID;
	const char* fragment_shader_filename;
	const char* vertex_shader_filename;
};
