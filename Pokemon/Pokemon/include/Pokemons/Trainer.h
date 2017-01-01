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
	virtual Pokemon* choosePokemonInBattle();

	bool checkIfTeamDead();
	std::vector<Pokemon*> getAvailablePokemon();
};

class MainTrainer : public Trainer {
public:
	std::vector<Pokemon*> pokemonInLab;
	int money;

	MainTrainer(const std::string trainerName);

	void takeTurnInBattle();
	Pokemon* choosePokemonInBattle();
	void notify(std::vector<void*> args);
private:
	std::vector< std::vector<void*> > notifications;
	std::mutex notificationMutex;
	std::condition_variable condVar;
	std::mutex condVarMutex;
};