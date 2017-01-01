#include "stdafx.h"
#include "Pokemons\Trainer.h"
#include "Game\Scene.h"

Trainer::Trainer(const std::string trainerName) 
	: name(trainerName) {}

Pokemon* Trainer::choosePokemonInBattle() {
	std::vector<Pokemon*> availablePokemon = getAvailablePokemon();
	Pokemon* chosen = nullptr;
	TrainerBattleScene* scene = (TrainerBattleScene*)Scene::getInstance();
	Pokemon* enemy;
	if (this == scene->trainerB)
		enemy = scene->pokemonA;
	else
		enemy = scene->pokemonB;

	if (enemy != nullptr) {
		double maxAdvantage = 0;

		for (auto &pokemon : availablePokemon) {
			double advantage = getTypeAdvantageOverPokemon(pokemon->type1, enemy->type1, enemy->type2);

			if (pokemon->type2 != Types::none)
				advantage = max(advantage, 
					getTypeAdvantageOverPokemon(pokemon->type2, enemy->type1, enemy->type2));

			if (advantage > maxAdvantage) {
				maxAdvantage = advantage;
				chosen = pokemon;
			}
		}
	}
	else {
		chosen = availablePokemon[random(0, (int)availablePokemon.size() - 1)];
	}

	return chosen;
}

void Trainer::takeTurnInBattle() {
	TrainerBattleScene* scene = (TrainerBattleScene*)Scene::getInstance();
	Pokemon* enemy = scene->pokemonA;
	Pokemon* me = scene->pokemonB;
	if (this == scene->trainerA)
		std::swap(me, enemy);

	scene->pokemonWantsToUseMove(me, &me->moves[0]);
}

std::vector<Pokemon*> Trainer::getAvailablePokemon() {
	std::vector<Pokemon*> availablePokemon;
	for (auto &pokemon : pokemonInTeam)
		if (!pokemon->isFainted())
			availablePokemon.push_back(pokemon);

	return availablePokemon;
}

bool Trainer::checkIfTeamDead() {
	if (getAvailablePokemon().empty())
		return true;

	return false;

}

MainTrainer::MainTrainer(const std::string trainerName) 
	: Trainer(trainerName) {}

void MainTrainer::notify(std::vector<void*> args) {
	std::lock_guard<std::mutex> lock(notificationMutex);
	notifications.push_back(args);
	condVar.notify_all();
}

Pokemon* MainTrainer::choosePokemonInBattle() {
	// wait until notified by an user command
	std::unique_lock<std::mutex> lock(condVarMutex);
	condVar.wait(lock, [&]() { return notifications.size() > 0; });

	const std::vector<void*> notification = notifications.back();
	notifications.pop_back();

	return pokemonInTeam[(int)notification[2]];
}

void MainTrainer::takeTurnInBattle() {
	// wait until notified by an user command
	std::unique_lock<std::mutex> lock(condVarMutex);
	condVar.wait(lock, [&]() { return notifications.size() > 0; });

	const std::vector<void*> notification = notifications.back();
	notifications.pop_back();

	TrainerBattleScene* scene = (TrainerBattleScene*)Scene::getInstance();

	if ((int)notification[1] == 0) {
		scene->switchOut(this, pokemonInTeam[(int)notification[2]]);
	}

	if ((int)notification[1] == 1) {
		scene->pokemonWantsToUseMove((Pokemon*)notification[2], (Attack*)notification[4]);
	}
}
