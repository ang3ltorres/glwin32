#pragma once
#include <minwindef.h>

class Game
{
public:
	unsigned int windowWidth;
	unsigned int windowHeight;

	Game(HINSTANCE &hInstance, int nCmdShow);
	Game(const Game&) = delete;
	~Game();

	void loop();
	void update();
	void draw();
};
