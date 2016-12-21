#pragma once

#include <vector>
#include <string>
#include "Types.h"

class Pokemon {
public:
	struct Genders {
		enum Gender {
			none,
			male,
			female
		};
	};

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

		static Personality getRandomPersonality();
	};

	struct Stats {
		double accuracy, evasiveness;
		int speed, hp, attack, defense, spattack, spdefense;

		Stats(const int HP, const int ATK, const int DEF, 
			const int SPATK, const int SPDEF, const int SPD);
	};

	struct Health {
		struct Trauma {
			enum Cause {
				unknown,
				battle,
				affection,
				love
			} const cause;

			union {
				int pokemonEntry;
				Pokemon* pokemon;
			};

			const double ageWhenTraumatized;

			Trauma(const double age, const Cause c);
			Trauma(const double age, const Cause c, const int entry);
			Trauma(const double age, const Cause c, Pokemon* const poke);
		};

		bool isSick;
		int hunger;
		double sanity, repugnance;
		std::vector<Trauma> traumas;

		int frozen, asleep, poisoned,
			burnt, paralyzed, confused;

		Health();
	};

	struct Traits {
		const double beauty, airheadedness,
			laziness, kindness, humor,
			luck, smartness;
		double selfesteem, courage, shyness,
			obedience;

		Traits();
	};

	struct Statistics {
		const std::string placeWhereCaught;
		int fightsWon, fightsLost, streak;
		int battlesWonInGym, battlesWonAgainstTrainers;
		double timeInPokeball;

		Statistics(const std::string place);
	};

public:
	std::string nickname;
	const int pokedexEntry;
	const Genders::Gender gender;
	const Personalities::Personality personality;
	const Types::Type type1, type2;
	bool shiny;

	double age;
	const double height;
	double weight, BMI;

	Stats currentStats;
	Health health;
	Traits traits;

	Statistics statistics;

	Pokemon(const int no, const std::string place = "", double crtAge = -1);
	static Pokemon* getStarter(const int no);

	double getFear();
	double getAnger();
	double getHappiness();
	double getDisgust();
	double getTrust();
	double getBoredom();

	bool inLove();
	bool isFainted();

	//double getWillToFight();
	//void Eat();

private:
	Stats maxStats;

	std::vector<Pokemon*> crushes;
	std::vector<Pokemon*> friends;
	std::vector<Pokemon*> rivals;

	bool hasCrushOn(const Pokemon* const pokemon);

	static std::string getNameByPokedexEntry(const int no);
	static Stats getStatsByPokedexEntry(const int no);
	static Genders::Gender getGenderByPokedexEntry(const int no);
	static Types::Type getTypeByPokedexEntry(const int no, const int typeNo);
	static std::pair<double, double> getMeasurementsByPokedexEntry(const int no);
};

namespace PokemonLimits {
	namespace Hunger {
		const int bitHungry = 5;
		const int hungry = 15;
		const int veryHungry = 25;
		const int starving = 50;
	};
};

