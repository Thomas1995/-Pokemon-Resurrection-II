#include "stdafx.h"
#include "Pokemons\Trainer.h"
#include "Game\Scene.h"

Trainer::Trainer(const std::string trainerName) 
	: name(trainerName) {}

void Trainer::takeTurnInBattle() {
	TrainerBattleScene* scene = (TrainerBattleScene*)Scene::getInstance();
	Pokemon* enemy;
	if (this == scene->trainerB)
		enemy = scene->pokemonA;
	else
		enemy = scene->pokemonB;

	scene->pokemonWantsToUseMove(pokemonInTeam[0], enemy, &pokemonInTeam[0]->moves[0]);
}

bool Trainer::checkIfTeamDead() {
	for (auto pokemon : pokemonInTeam)
		if (!pokemon->isFainted())
			return false;

	return true;

}

MainTrainer::MainTrainer(const std::string trainerName) 
	: Trainer(trainerName) {}

void MainTrainer::notify(std::vector<void*> args) {
	std::lock_guard<std::mutex> lock(notificationMutex);
	notifications.push_back(args);
	condVar.notify_all();
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
		scene->pokemonWantsToUseMove((Pokemon*)notification[2],
			(Pokemon*)notification[3], (Attack*)notification[4]);
	}
}
