#include "stdafx.h"
#include "Game\Game.h"
#include "Game\Scene.h"
#include "Pokemons\Pokemon.h"
#include "Pokemons\Trainer.h"

int main() {
	/*
		Comment the line above to check stderr messages in the console window.
		However, it should not appear as a part of the game.
	*/
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	Game::initialize(Size<unsigned int>(800, 600), false)->start();

	return 0;
}