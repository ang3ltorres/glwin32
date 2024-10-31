#include "game.hpp"

#include "window.hpp"
#include "graphics.hpp"
#include "glext.hpp"

void resized(unsigned int width, unsigned int height)
{

}

Game::Game(HINSTANCE &hInstance, int nCmdShow)
{
	Window::initialize(800, 600, hInstance, nCmdShow);
	Window::resizedCallback = &resized;
	Window::resizedCallback(800, 600);

	Graphics::initialize();
}

Game::~Game()
{
	Graphics::finalize();
	Window::finalize();
}

void Game::loop()
{
	while (!Window::shouldClose())
	{
		Game::update();
		Game::draw();

		Window::resetInput();
	}
}

void Game::update()
{

}

void Game::draw()
{
	glViewport(0, 0, 800, 600);

	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// swap the buffers to show output
	SwapBuffers(Graphics::dc);
}