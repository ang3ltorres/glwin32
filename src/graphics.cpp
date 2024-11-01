#include "graphics.hpp"
#include "window.hpp"
#include "glext.hpp"

HGLRC Graphics::rc;
HDC Graphics::dc;

void Graphics::initialize()
{
	Glext::loadExtensionsWGL();

	int piAttribIList[] =
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
	Graphics::dc = GetDC(Window::hwnd);
	wglChoosePixelFormatARB(Graphics::dc, piAttribIList, nullptr, 1, &format, &formats);

	PIXELFORMATDESCRIPTOR desc = {
		.nSize = sizeof(PIXELFORMATDESCRIPTOR),
		.nVersion = 1,
		.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		.iPixelType = PFD_TYPE_RGBA,
		.cColorBits = 24,
	};

	DescribePixelFormat(Graphics::dc, format, sizeof(PIXELFORMATDESCRIPTOR), &desc);
	SetPixelFormat(Graphics::dc, format, &desc);

	int attribList[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0,
	};

	Graphics::rc = wglCreateContextAttribsARB(Graphics::dc, nullptr, attribList);
	wglMakeCurrent(Graphics::dc, Graphics::rc);

	Glext::loadExtensionsGL();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	wglSwapIntervalEXT(1);
}

void Graphics::finalize()
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(Graphics::rc);
    ReleaseDC(Window::hwnd, Graphics::dc);
}

void Graphics::clearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::setClearScreenColor(const Color &color)
{
	glClearColor(float{color.r} / 255.0f, float{color.g} / 255.0f, float{color.b} / 255.0f, float{color.a} / 255.0f);
}

void Graphics::setViewport(unsigned int width, unsigned int height)
{
	glViewport(0, 0, width, height);
}

void Graphics::swapBuffers()
{
	SwapBuffers(Graphics::dc);
}
