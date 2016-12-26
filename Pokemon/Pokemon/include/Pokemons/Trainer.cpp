#include "stdafx.h"
#include "Trainer.h"
#include "Game\Scene.h"

Trainer::Trainer(const std::string trainerName) 
	: name(trainerName) {}

void Trainer::takeTurnInBattle() {}

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
	scene->switchOut(this, pokemonInTeam[(int)notification[1]]);
}
