#include "stdafx.h"
#include <glew/glew.h>
#include "Game.h"
#include <ctime>
#include <random>
#include "stdio.h"
#include <Windows.h>


int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	srand((unsigned int)time(0));

	Game* game = new Game();
	game->initialize();

	while (game->update())
	{

	}

	delete game;

	return 0;
}