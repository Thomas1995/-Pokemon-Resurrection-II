#pragma once

#include "Pokemon.h"
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

class Trainer {
public:
	std::string name;
	std::vector<Pokemon*> pokemonInTeam;

	Trainer(const std::string trainerName);

	virtual void takeTurnInBattle();
};

class MainTrainer : public Trainer {
public:
	std::vector<Pokemon*> pokemonInLab;
	int money;

	MainTrainer(const std::string trainerName);

	void takeTurnInBattle();
	void notify(std::vector<void*> args);
private:
	std::vector< std::vector<void*> > notifications;
	std::mutex notificationMutex;
	std::condition_variable condVar;
	std::mutex condVarMutex;
};