#pragma once

#include "Pokemon.h"
#include <string>
#include <vector>

class Trainer {
public:
	std::string name;
	std::vector<Pokemon*> pokemonInTeam;

	Trainer(const std::string trainerName);
};

class MainTrainer : public Trainer {
public:
	std::vector<Pokemon*> pokemonInLab;

	MainTrainer(const std::string trainerName);
};