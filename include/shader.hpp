#pragma once

class Shader
{
public:
	Shader(const char *vertexShader, const char *fragmentShader);
	~Shader();

	void use() const;
	unsigned int program;
};
