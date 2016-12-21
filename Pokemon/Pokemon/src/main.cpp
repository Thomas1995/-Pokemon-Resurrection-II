#include "stdafx.h"
#include "Game\Game.h"
#include "Pokemons\Pokemon.h"

int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	Pokemon* pokemon = Pokemon::getStarter(57);

	Game::initialize(Size(800, 600), false)->start();

	return 0;
}

