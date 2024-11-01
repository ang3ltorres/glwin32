#pragma once

#include "utils.hpp"

#include <windef.h>


class Graphics
{
public:
	Graphics() = delete;
	Graphics(const Graphics&) = delete;
	~Graphics() = default;
	
	static void initialize();
	static void finalize();
	
	static HGLRC rc;
	static HDC dc;

	static void clearScreen();
	static void setClearScreenColor(const Color &color);
	static void setViewport(unsigned int width, unsigned int height);
};