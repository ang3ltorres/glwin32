#pragma once

class Texture
{
public:
	unsigned int id;
	unsigned int width;
	unsigned int height;

	static void initialize();
	static void finalize();

	Texture(const char *fileName);
	~Texture();
};