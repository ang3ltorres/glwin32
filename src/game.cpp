#include "game.hpp"

#include "window.hpp"
#include "graphics.hpp"

#include "shader.hpp"

void resized(unsigned int width, unsigned int height)
{
	Graphics::setViewport(width, height);
}

Game::Game(HINSTANCE &hInstance, int nCmdShow)
{
	unsigned int width = 800;
	unsigned int height = 600;

	Window::initialize(width, height, "OpenGL", hInstance, nCmdShow);
	Window::resizedCallback = &resized;
	Window::resizedCallback(width, height);

	Graphics::initialize();
	Graphics::setClearScreenColor({255, 0, 0, 255});

	Shader s{"../shader/vertex.vs", "../shader/fragment.fs"};
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
	Graphics::clearScreen();

	Graphics::swapBuffers();
}