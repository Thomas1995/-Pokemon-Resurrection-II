#include "stdafx.h"
#include "Game\Game.h"
#include "Game\Scene.h"
#include "Pokemons\Pokemon.h"
#include "Pokemons\Trainer.h"

int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	Pokemon* pokemon1 = Pokemon::getStarter(1);
	Pokemon* pokemon2 = Pokemon::getStarter(4);
	pokemon1->currentStats.hp = 0;
	Trainer me("Thomas"), enemy("Cristian");
	me.pokemonInTeam.push_back(pokemon1);
	me.pokemonInTeam.push_back(new Pokemon(151));
	me.pokemonInTeam.push_back(new Pokemon(235));
	me.pokemonInTeam.push_back(new Pokemon(422));
	me.pokemonInTeam.push_back(new Pokemon(345));
	me.pokemonInTeam.push_back(new Pokemon(554));
	enemy.pokemonInTeam.push_back(pokemon2);

	Scene::updateInstance(new TrainerBattleScene(&me, &enemy));
	Game::initialize(Size<unsigned int>(800, 600), false)->start();

	return 0;
}