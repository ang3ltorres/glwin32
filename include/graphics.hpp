#pragma once

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
};