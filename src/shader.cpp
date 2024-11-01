#include "shader.hpp"
#include "glext.hpp"

#include <cstdio>

static char* readFile(const char *fileName)
{
	FILE *file; 
	long size;
	char *text;

	file = fopen(fileName, "rb");
	fseek(file, 0L, SEEK_END);
	size = ftell(file);
	text = new char[size + 1];
	text[size] = '\0';
	fseek(file, 0L, SEEK_SET);
	fread(text, 1, size, file);
	fclose(file);
	return text;
}

static unsigned int compileShader(unsigned int type, const char *source)
{
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		printf("Shader compilation error:\n%s\n", infoLog);
	}
	return shader;
}

Shader::Shader(const char *vertexShader, const char *fragmentShader)
{
	char *vertexCode = readFile(vertexShader);
	char *fragmentCode = readFile(fragmentShader);

	unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexCode);
	unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(program, 512, nullptr, infoLog);
			printf("Shader linking error:\n%s\n", infoLog);
	}

	// Delete the shaders as they’re linked into the program now and no longer needed
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	delete[] vertexCode;
	delete[] fragmentCode;
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

void Shader::use() const
{
  glUseProgram(program);
}
