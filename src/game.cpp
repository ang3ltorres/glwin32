#include "game.hpp"

#include "window.hpp"
#include "graphics.hpp"
#include "glext.hpp"

void resized(unsigned int width, unsigned int height)
{

}

Game::Game(HINSTANCE &hInstance, int nCmdShow)
{
	Window::initialize(800, 600, "OpenGL", hInstance, nCmdShow);
	Window::resizedCallback = &resized;
	Window::resizedCallback(800, 600);

	Graphics::initialize();
	Graphics::setClearScreenColor({255, 0, 0, 255});
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
	Graphics::clearScreen();

	// swap the buffers to show output
	SwapBuffers(Graphics::dc);
}