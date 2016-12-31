#pragma once

#include <string>
#include "Pokemons\Types.h"

extern class Pokemon;

struct Categories {
	enum Category {
		physical,
		special,
		status
	};
};

class Effect;

class Attack {
public:
	std::string name, description;
	int damage, accuracy, ppmax;
	int pp;
	int(*damageFormula)(Pokemon* attacker, Pokemon* target) = nullptr;

	Categories::Category category;
	Types::Type type;

	int getDamage(Pokemon* attacker, Pokemon* target);

	bool isUsable();
	static Attack get(std::string name);
	std::vector<Effect> effects;
};

class Effect {
public:
	bool active;
	Pokemon *attacker, *target;
	Attack *attack;
	int damage, turns, repeats;
	double chance, criticalChance;
	void(*specialEffect)(Effect* effect);

	Effect();
	void activate();
};