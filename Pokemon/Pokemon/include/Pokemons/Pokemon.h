#pragma once

#include <list>
#include <string>

enum Gender {
	none,
	male,
	female
};

enum Type {
	none,
	normal,
	fire,
	water,
	electricity,
	grass,
	ice,
	fight,
	poison,
	ground,
	flying,
	psychic,
	bug,
	rock,
	ghost,
	dragon,
	dark,
	steel,
	fairy
};

class Pokemon {

	struct Personalities {
		enum Personality {
			friendly,
			flirty,
			leader,
			grumpy,
			melancholic,
			hothead,
			stubborn
		};
	};

	struct Stats {
		int speed, accuracy, evasiveness,
			hp, attack, defense, spattack, spdefense;
	};

	struct Health {
		struct Trauma {
			enum Cause {
				unknown,
				pokemon,
				affection,
				love
			} cause;

			union {
				int pokemonEntry;
				Pokemon* pokemon;
			};

			const double ageWhenTraumatized;
		};

		bool isSick;
		int hunger;
		double sanity;
		std::list<Trauma> traumas;

		int frozen, asleep, poisoned,
			burnt, paralyze, confused;
	};

	struct Traits {
		const double beauty, airheadedness,
			laziness, kindness, humor,
			luck, smartness;
		double selfesteem, courage, shyness,
			obedience;
	};

	struct Statistics {
		std::string placeWhereCaught;
		int fightsWon, fightsLost;
	};

	std::string nickname;
	int pokedexEntry;
	Gender gender;
	Personalities::Personality personality;
	Type type1, type2;
	double age;
	const double height;
	double weight, BMI;

	Stats currentStats, maxStats;
	Health health;
	Traits traits;

	Statistics statistics;
	
	std::list<Pokemon*> crushes;
	std::list<Pokemon*> friends;
	std::list<Pokemon*> rivals;

	double getFear();
	double getAnger();
	double getHappiness();
	double getDisgust();
	double getTrust();
	double getBoredom();

	double getWillToFight();
	bool inLove();

	void Eat();
};

namespace PokemonLimits {
	namespace Hunger {
		const int bitHungry = 5;
		const int hungry = 15;
		const int veryHungry = 25;
		const int starving = 50;
	};
};