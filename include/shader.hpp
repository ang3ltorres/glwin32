#pragma once

class Shader
{
public:
	Shader(const char *vertexShader, const char *fragmentShader);
	~Shader() = default;

	unsigned int pipeline;
	unsigned int vshader;
	unsigned int fshader;
};
