#include "stdafx.h"
#include "Game\Game.h"

int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	Game::initialize(Size(800, 600), false)->start();

	return 0;
}

