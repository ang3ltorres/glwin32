#include "window.hpp"

#include <objbase.h>

bool Window::forceClose = false;
void (*Window::resizedCallback)(unsigned int width, unsigned int height) = nullptr;

WNDCLASSEXA Window::windowClass;
HWND Window::hwnd;
MSG Window::msg;
LONG_PTR Window::savedStyle;
RECT Window::savedRect;

unsigned int Window::width = 0;
unsigned int Window::height = 0;

unsigned int Window::scale = 1;
unsigned int Window::offsetX = 0;
unsigned int Window::offsetY = 0;

bool Window::up[3];
bool Window::down[3];
bool Window::left[3];
bool Window::right[3];

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (uMsg)
	{
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_SIZE:
		{
			RECT rc;

			GetClientRect(Window::hwnd, &rc);

			Window::width = rc.right - rc.left;
			Window::height = rc.bottom - rc.top;

			if (Window::resizedCallback)
				Window::resizedCallback(Window::width, Window::height);

			return 0;
		}

		case WM_KEYDOWN:
		{
			Window::keyDown(wParam);
			return 0;
		}

		case WM_KEYUP:
		{
			Window::keyUp(wParam);
			return 0;
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Window::initialize(int width, int height, const char *title, HINSTANCE hInstance, int nCmdShow)
{
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

	Window::width = width;
	Window::height = height;

	ZeroMemory(&windowClass, sizeof(WNDCLASSEXA));
	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.hInstance = hInstance;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.lpszClassName = "MainWindow";
	windowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassExA(&windowClass);
	RECT rect = { 0, 0, {Window::width}, {Window::height} };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

	// Calculate window position to center it on the screen
	int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int posX = (screenWidth - (rect.right - rect.left)) / 2;
	int posY = (screenHeight - (rect.bottom - rect.top)) / 2;

	Window::hwnd = CreateWindowExA(
		WS_EX_OVERLAPPEDWINDOW, windowClass.lpszClassName, title, WS_OVERLAPPEDWINDOW,
		posX, posY, rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, hInstance, nullptr
	);
	
	ShowWindow(Window::hwnd, SW_NORMAL);
	SetForegroundWindow(Window::hwnd);
	SetActiveWindow(Window::hwnd);
	SetFocus(Window::hwnd);

	savedStyle = GetWindowLongPtr(hwnd, GWL_STYLE);
	GetWindowRect(hwnd, &savedRect);

	ZeroMemory(&Window::msg, sizeof(MSG));
	Window::msg.message = WM_NULL;
}

void Window::finalize()
{
	UnregisterClassA(windowClass.lpszClassName, windowClass.hInstance);
	DestroyWindow(Window::hwnd);

	CoUninitialize();
}

bool Window::shouldClose()
{
	if (forceClose)
		return true;

	while (PeekMessage(&Window::msg, NULL, 0, 0, PM_REMOVE))
	{
		if (Window::msg.message == WM_QUIT)
			return true;

		TranslateMessage(&Window::msg);
		DispatchMessage(&Window::msg);
	}

	return false;
}

void Window::keyDown(WPARAM wParam)
{
	switch (wParam)
	{
		case VK_UP:
			if (!Window::up[0]) {
					Window::up[0] = true;
					Window::up[1] = true;
			}
			break;

		case VK_DOWN:
			if (!Window::down[0]) {
					Window::down[0] = true;
					Window::down[1] = true;
			}
			break;

		case VK_LEFT:
			if (!Window::left[0]) {
					Window::left[0] = true;
					Window::left[1] = true;
			}
			break;
				
		case VK_RIGHT:
			if (!Window::right[0]) {
					Window::right[0] = true;
					Window::right[1] = true;
			}
			break;
	}
}

void Window::keyUp(WPARAM wParam)
{
	switch (wParam)
	{
		case VK_UP:
			Window::up[0] = false;
			Window::up[2] = true;
			break;

		case VK_DOWN:
			Window::down[0] = false;
			Window::down[2] = true;
			break;

		case VK_LEFT:
			Window::left[0] = false;
			Window::left[2] = true;
			break;
				
		case VK_RIGHT:
			Window::right[0] = false;
			Window::right[2] = true;
			break;
	}
}

void Window::resetInput()
{
	Window::up[1] = false;
	Window::down[1] = false;
	Window::left[1] = false;
	Window::right[1] = false;

	Window::up[2] = false;
	Window::down[2] = false;
	Window::left[2] = false;
	Window::right[2] = false;
}