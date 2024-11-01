#pragma once

#include <windef.h>
#include <winuser.h>

class Window
{
public:
	Window() = delete;
	Window(const Window&) = delete;
	~Window() = default;

	static void initialize(int width, int height, const char *title, HINSTANCE hInstance, int nCmdShow);
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

	static bool up[3];
	static bool down[3];
	static bool left[3];
	static bool right[3];

	static void keyDown(WPARAM wParam);
	static void keyUp(WPARAM wParam);
	static void resetInput();
};
