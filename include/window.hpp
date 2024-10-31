#pragma once

#include <windef.h>
#include <winuser.h>

class Window
{
public:
	Window() = delete;
	Window(const Window&) = delete;
	~Window() = default;

	static void initialize(int width, int height, HINSTANCE hInstance, int nCmdShow);
	static void finalize();

	static bool shouldClose();
	static bool forceClose;

	static void (*resizedCallback)(unsigned int width, unsigned int height);
	
	static WNDCLASSEXA windowClass;
	static HWND hwnd;
	static MSG msg;
	static LONG_PTR savedStyle;
	static RECT savedRect;

	static unsigned int width;
	static unsigned int height;

	static unsigned int scale;
	static unsigned int offsetX;
	static unsigned int offsetY;
};
