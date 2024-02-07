#include "shader.h"
#include <fstream>
#include <iostream>

Shader::Shader(const char* vertexShaderFilename, const char* fragmentShaderFilename) {
	vertex_shader_filename = vertexShaderFilename;
	fragment_shader_filename = fragmentShaderFilename;
}


Shader::~Shader() {
	glDeleteProgram(shaderID);
}

void Shader::init() {
	shaderID = createShader(vertex_shader_filename, fragment_shader_filename);
}

void Shader::bind() {
	glUseProgram(shaderID);
}
void Shader::unbind() {
	glUseProgram(0);
}

GLint Shader::getUniformLocation(const GLchar* name) {
	bind();
	return glGetUniformLocation(shaderID, name);
}

GLuint Shader::compile(std::string shaderSource, GLenum shaderType) {
	GLuint id = glCreateShader(shaderType);
	const char* src = shaderSource.c_str();

	glShaderSource(id, 1, &src, 0);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE) {
		// Shader konnte nicht kompiliert werden, weil es verm. ein Compile-Fehler gab
		int length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = new char[length];

		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Fehler beim kompilieren eines Shaders!\n" << message << std::endl;
		delete[] message;
		return 0;
	}

	return id;
}

std::string Shader::parse(const char* filename) {
	FILE* file;

#ifdef _WIN32
	if (fopen_s(&file, filename, "rb") != 0) {
		std::cout << "File " << filename << " not found" << std::endl;
		return "";
	}
#else
	file = fopen(filename, "rb");
	if (file == nullptr) {
		std::cout << "File " << filename << " not found" << std::endl;
		return "";
	}
#endif

	if (file == nullptr) {
		std::cout << "Shader file not found!" << std::endl;
		return "";
	}

	std::string content;
	fseek(file, 0, SEEK_END); // Ans Ende der Datei gehen
	size_t filesize = ftell(file); // Größe der Datei feststellen. Wo sind wir in der Datei?

	rewind(file); // In der Datei wieder zurück gehen
	content.resize(filesize);

	fread(&content[0], 1, filesize, file);

	return content;
}

GLuint Shader::createShader(const char* vertexShaderFilename, const char* fragmentShaderFilename) {
	std::string vertexShaderSource = parse(vertexShaderFilename);
	std::string fragmentShaderSource = parse(fragmentShaderFilename);

	GLuint program = glCreateProgram();
	GLuint vertexShader = compile(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint fragmentShader = compile(fragmentShaderSource, GL_FRAGMENT_SHADER);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program); // Shader sind gelinkt, Shader können wieder deattached werden oder gelöscht werden, ShaderProgram bleibt

#ifdef _RELEASE
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
#endif // Release Modus

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		std::cout << "Fehler beim erstellen eines Shaders!\n" << std::endl;
		return 0;
	}

	return program;
}