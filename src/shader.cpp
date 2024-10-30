#include "shader.hpp"

#include <cstdio>
#include "glext.hpp"

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

Shader::Shader(const char *vertexShader, const char *fragmentShader)
{
	char *vertexCode = readFile(vertexShader);
	char *fragmentCode = readFile(fragmentShader);

	vshader = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vertexCode);
	fshader = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fragmentCode);

	glGenProgramPipelines(1, &pipeline);
	glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vshader);
	glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fshader);

	delete[] vertexCode;
	delete[] fragmentCode;
}
