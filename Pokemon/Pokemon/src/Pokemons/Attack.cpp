#include "stdafx.h"
#include "Pokemons\Attack.h"
#include "Pokemons\Pokemon.h"
#include <unordered_map>
#include <fstream>

bool Attack::isUsable() {
	return pp > 0;
}

int Attack::getDamage(Pokemon* attacker, Pokemon* target) {
	if (damage > 0)
		return damage;

	if (damageFormula != nullptr)
		return damageFormula(attacker, target);

	return 0;
}

Attack Attack::get(std::string name) {
	static std::unordered_map<std::string, Attack> moves = [&]() {
		static std::unordered_map<std::string, Attack> moves;
		std::ifstream file(resourcesFolder + "Bins\\moves.bin",
			std::ios::binary | std::ios::in);

		std::string str;
		int type, category;

		while (file >> str) {
			Attack move;

			move.name = str;

			file >> type >> category;
			move.type = (Types::Type)type;
			move.category = (Categories::Category)category;

			file >> move.damage >> move.accuracy >> move.ppmax;
			move.pp = move.ppmax;

			file >> move.description;

			moves[str] = move;
		}

		file.close();
		return moves;
	}();

	return moves[name];
}

Effect::Effect() {
	active = true;
	specialEffect = nullptr;
	turns = 1;
	repeats = 1;
	chance = 1;
}

void Effect::activate() {
}
