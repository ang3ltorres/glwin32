#include <windows.h>

#define _USE_MATH_DEFINES
#include <cmath>

#include "shader.hpp"
#include "glext.hpp"

static LRESULT CALLBACK WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcW(wnd, msg, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previnstance, LPSTR cmdline, int cmdshow)
{
	// get WGL functions to be able to create modern GL context
	Glext::loadExtensionsWGL();

	// register window class to have custom WindowProc callback
	WNDCLASSEXW wc =
	{
		.cbSize = sizeof(WNDCLASSEXW),
		.lpfnWndProc = WindowProc,
		.hInstance = instance,
		.hIcon = LoadIcon(nullptr, IDI_APPLICATION),
		.hCursor = LoadCursor(nullptr, IDC_ARROW),
		.lpszClassName = L"opengl_window_class",
	};
	RegisterClassExW(&wc);

	// window properties - width, height and style
	int width = CW_USEDEFAULT;
	int height = CW_USEDEFAULT;
	DWORD exstyle = WS_EX_APPWINDOW;
	DWORD style = WS_OVERLAPPEDWINDOW;

	// uncomment in case you want fixed size window
	//style &= ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
	//RECT rect = { 0, 0, 1280, 720 };
	//AdjustWindowRectEx(&rect, style, FALSE, exstyle);
	//width = rect.right - rect.left;
	//height = rect.bottom - rect.top;

	// create window
	HWND window = CreateWindowExW(
		exstyle, wc.lpszClassName, L"OpenGL Window", style,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, wc.hInstance, nullptr);

	HDC dc = GetDC(window);

	// set pixel format for OpenGL context
	{
		int attrib[] =
		{
			WGL_DRAW_TO_WINDOW_ARB,           GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,           GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,            GL_TRUE,
			WGL_PIXEL_TYPE_ARB,               WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,               24,
			WGL_DEPTH_BITS_ARB,               24,
			WGL_STENCIL_BITS_ARB,             8,
			WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
			0,
		};

		int format;
		UINT formats;
		wglChoosePixelFormatARB(dc, attrib, nullptr, 1, &format, &formats);


		PIXELFORMATDESCRIPTOR desc = {
			.nSize = sizeof(PIXELFORMATDESCRIPTOR),
			.nVersion = 1,
			.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			.iPixelType = PFD_TYPE_RGBA,
			.cColorBits = 24,
		};

		DescribePixelFormat(dc, format, sizeof(PIXELFORMATDESCRIPTOR), &desc);
		SetPixelFormat(dc, format, &desc);
	}

	// create modern OpenGL context
	{
		int attrib[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 6,
			WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0,
		};

		HGLRC rc = wglCreateContextAttribsARB(dc, nullptr, attrib);

		wglMakeCurrent(dc, rc);

		// load OpenGL functions
		Glext::loadExtensionsGL();
	}

	// vertex buffer containing triangle vertices
	GLuint vbo;
	{
		float vertices[] =
		{
			-1.0f, +1.0f,
			+1.0f, +1.0f,
			+0.0f, -1.0f,
		};

		glCreateBuffers(1, &vbo);
		glNamedBufferStorage(vbo, sizeof(vertices), vertices, 0);
	}

	// vertex input
	GLuint vao;
	{
		glCreateVertexArrays(1, &vao);

		GLint vbuf_index = 0;
		glVertexArrayVertexBuffer(vao, vbuf_index, vbo, 0, sizeof(float) * 2);

		GLint a_pos = 0;
		glVertexArrayAttribFormat(vao, a_pos, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(vao, a_pos, vbuf_index);
		glEnableVertexArrayAttrib(vao, a_pos);
	}

	Shader s{"../shader/vertex.vs", "../shader/fragment.fs"};

	// enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// disble depth testing
	glDisable(GL_DEPTH_TEST);

	// disable culling
	glDisable(GL_CULL_FACE);

	// set to FALSE to disable vsync
	bool vsync = true;
	wglSwapIntervalEXT(vsync ? 1 : 0);

	// show the window
	ShowWindow(window, SW_SHOWDEFAULT);

	LARGE_INTEGER freq, c1;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&c1);

	float angle = 0;

	while (true)
	{
		// process all incoming Windows messages
		MSG msg;
		if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			continue;
		}

		// get current window client area size
		RECT rect;
		GetClientRect(window, &rect);
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;

		LARGE_INTEGER c2;
		QueryPerformanceCounter(&c2);
		float delta = (float)((double)(c2.QuadPart - c1.QuadPart) / freq.QuadPart);
		c1 = c2;

		// setup output size covering all client area of window
		glViewport(0, 0, width, height);

		// clear screen
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// setup rotation matrix in uniform
		{
			angle += delta * 2.0f * (float)M_PI / 20.0f; // full rotation in 20 seconds
			angle = fmodf(angle, 2.0f * (float)M_PI);

			float aspect = (float)height / width;
			float matrix[] =
			{
				cosf(angle) * aspect, -sinf(angle),
				sinf(angle) * aspect,  cosf(angle),
			};

			GLint u_matrix = 0;
			glProgramUniformMatrix2fv(s.vshader, u_matrix, 1, GL_FALSE, matrix);
		}

		// activate shaders for next draw call
		glBindProgramPipeline(s.pipeline);

		// provide vertex input
		glBindVertexArray(vao);

		// draw 3 vertices as triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// swap the buffers to show output
		SwapBuffers(dc);
	}

	return 0;
}