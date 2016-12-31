#include "stdafx.h"
#include "Pokemons\Pokemon.h"
#include "Game\Scene.h"
#include <fstream>

double Pokemon::getHappiness() {
	auto f = [](const size_t x) {
		double y = (double)x;
		return y / (0.916 * (y + 0.916));
	};

	// happiness formula: hight selfesteem, trust in trainer, friends and a bit of ignorance
	double happiness = percentage(traits.selfesteem, 40) + percentage(getTrust(), 30) +
		percentage(f(friends.size()), 20) + percentage(1 - traits.smartness, 10);

	// hunger lowers happiness (depending on body mass index)
	happiness -= percentage(
		normalize(health.hunger, 0, PokemonLimits::Hunger::starving) / PokemonLimits::Hunger::starving,
		(int)normalize(BMI / 2.0, 0, 50)
	);

	// other feelings also influence this
	double(Pokemon::*otherFeelingsFcts[])() = {
		&Pokemon::getBoredom,
		&Pokemon::getDisgust,
		&Pokemon::getFear,
		&Pokemon::getAnger,
	};

	for (int i = 0; i < 4; ++i) {
		double p = (this->*otherFeelingsFcts[i])();
		if (p > 0.5) {
			happiness -= percentage(p, 8 * i);
		}
	}

	// insanity happiness bonus
	happiness += percentage(1 - health.sanity, 20);

	// a grumpy or melancholic pokemon is sadder
	if (personality == Personalities::grumpy ||
		personality == Personalities::melancholic)
		happiness -= 0.2;

	// sickness deters happiness
	if (health.isSick)
		happiness /= 2.0;

	// everything is greater when in love
	if (inLove())
		happiness *= 1.5;

	return normalize(happiness);
}

double Pokemon::getDisgust() {
	// pokemon get disgusted by things they find repugnant
	double disgust = health.repugnance;

	// friendly pokemons get disgusted when beating too much pokemon
	if (personality == Personalities::friendly) {
		if (statistics.streak > 0) {
			disgust += 0.1 * statistics.streak;
		}
	}

	// females get disgusted easily
	if (gender == Genders::female)
		disgust *= 1.5;

	return normalize(disgust);
}

double Pokemon::getTrust() {
	// trust depends on the results of the fights
	double trust = normalize((double)statistics.fightsWon / (2.0 * statistics.fightsLost + 1));

	// trust raises when used in gym battles or versus other trainers
	trust += 0.1 * statistics.battlesWonInGym + 0.01 * statistics.battlesWonAgainstTrainers;

	// an insane pokemon does not trust trainer so much
	trust -= health.sanity / 2.0;

	// stubborn pokemon trust trainer less
	if (personality == Personalities::stubborn)
		trust -= 0.2;

	// friendly pokemon trust trainer more
	if (personality == Personalities::friendly)
		trust += 0.2;
		
	return normalize(trust);
}

double Pokemon::getBoredom() {
	// pokemon gets bored in pokeball
	double boredom = statistics.timeInPokeball / 2.0;

	// a lazy pokemon do not get so bored
	boredom -= traits.laziness / 2.0;

	return normalize(boredom);
}

double Pokemon::getFear() {
	// fear comes from not trusting the trainer, lack of courage and shyness
	double fear = percentage(1 - traits.courage, 20) + percentage(traits.shyness, 10) +
		percentage(1 - getTrust(), 20);
	
	// not having friends leads to fear
	if (friends.empty())
		fear += 0.25;

	// traumas inspire fear
	for (auto trauma : health.traumas) {
		double time = age - trauma.ageWhenTraumatized;

		// recent trauma
		if (time <= 1)
			fear += 0.1;
		else
			fear += 0.02;
	}

	// a leader inspire courage
	if (personality == Personalities::leader)
		fear -= 0.2;

	return normalize(fear);
}

double Pokemon::getAnger() {
	double anger = 0;

	// pokemon gets angry if he loses too much fights
	if (statistics.streak < 0) {
		anger += 0.1 * (-statistics.streak);
	}

	// having rivals raises anger
	anger += 0.05 * rivals.size();

	// a hothead gets angry easily
	if (personality == Personalities::hothead)
		anger += 0.2;

	return normalize(anger);
}

double Pokemon::getWillToFight() {
	// it needs trust or anger to get back on feet
	double willToFight = max(getAnger(), getTrust());

	// the courage raises the will to fight
	willToFight += percentage(traits.courage, 20);

	// a stubborn pokemon tends to get again on his feet more
	if (personality == Personalities::stubborn)
		willToFight += 0.2;

	return normalize(willToFight / ++timesBackOnFeet);
}

bool Pokemon::hasCrushOn(const Pokemon* const pokemon) {
	if (std::find(crushes.begin(), crushes.end(), pokemon) != crushes.end())
		return true;
	return false;
}

bool Pokemon::inLove() {
	for (auto crush : crushes)
		if (crush->hasCrushOn(this))
			return true;

	return false;
}

bool Pokemon::isFainted() {
	return (currentStats.hp == 0);
}

std::vector<Attack*> Pokemon::getAvailableMoves() {
	std::vector<Attack*> availableMoves;

	for (int i = 0; i < moves.size(); ++i)
		if (moves[i].isUsable())
			availableMoves.push_back(&moves[i]);

	return availableMoves;
}

Pokemon::Statistics::Statistics(const std::string place) 
	: placeWhereCaught(place), fightsWon(0), fightsLost(0),
	streak(0), battlesWonInGym(0), battlesWonAgainstTrainers(0),
	timeInPokeball(0) {}

Pokemon::Health::Trauma::Trauma(const double age, const Cause c) 
	: ageWhenTraumatized(age), cause(c) {}

Pokemon::Health::Trauma::Trauma(const double age, const Cause c, const int entry) 
	: Trauma(age, c) {
	pokemonEntry = entry;
}

Pokemon::Health::Trauma::Trauma(const double age, const Cause c, Pokemon* const poke) 
	: Trauma(age, c) {
	pokemon = poke;
}

Pokemon::Health::Health() 
	: isSick(false), hunger(0), repugnance(0),
	sanity(random(0.0, 0.6)) {}

Pokemon::Stats::Stats(const int HP, const int ATK, const int DEF, 
	const int SPATK, const int SPDEF, const int SPD)
	: speed(SPD), hp(HP), attack(ATK), defense(DEF), 
	spattack(SPATK), spdefense(SPDEF), 
	accuracy(1), evasiveness(1) {}

Pokemon::Traits::Traits()
	: beauty(random(0.0, 1.0)),
	airheadedness(random(0.0, 1.0)),
	laziness(random(0.0, 1.0)),
	kindness(random(0.0, 1.0)),
	humor(random(0.0, 1.0)),
	luck(random(0.0, 1.0)),
	smartness(random(0.0, 1.0)),
	selfesteem(random(0.0, 0.6)),
	courage(random(0.0, 0.9)),
	shyness(random(0.0, 1.0)),
	obedience(random(0.0, 0.5)) {}

Pokemon::Personalities::Personality Pokemon::Personalities::getRandomPersonality() {
	return (Pokemon::Personalities::Personality)random(0, 6);
}

Pokemon::Pokemon(const int no, const std::string place, double crtAge)
	: pokedexEntry(no), statistics(place),
	nickname(getNameByPokedexEntry(no)),
	currentStats(getStatsByPokedexEntry(no)),
	maxStats(getStatsByPokedexEntry(no)),
	gender(getGenderByPokedexEntry(no)),
	type1(getTypeByPokedexEntry(no, 1)),
	type2(getTypeByPokedexEntry(no, 2)),
	height(getMeasurementsByPokedexEntry(no).first),
	weight(getMeasurementsByPokedexEntry(no).second),
	personality(Personalities::getRandomPersonality()) {
	
	// age is not given
	if (crtAge == -1)
		age = random(0.5, 10.0);
	else
		age = crtAge;

	BMI = weight / (height * height);

	shiny = random(1, 100) < 99 ? false : true;
}

Pokemon* Pokemon::getStarter(const int no) {
	Pokemon* pokemon = new Pokemon(no, "Laboratory", 1);
	pokemon->traits.obedience = 1;
	pokemon->shiny = true;
	return pokemon;
}

std::string Pokemon::getNameByPokedexEntry(const int no) {
	static std::vector<std::string> names = []() {
		std::vector<std::string> names(maxPokedexEntry);
		std::ifstream file(resourcesFolder + "Bins\\names.bin");

		for (int i = 0; i < names.size(); ++i)
			file >> names[i];

		file.close();
		return names;
	}();

	return names[no - 1];
}

Pokemon::Stats Pokemon::getStatsByPokedexEntry(const int no) {
	static std::vector<int[6]> stats = []() {
		std::vector<int[6]> stats(maxPokedexEntry);
		std::ifstream file(resourcesFolder + "Bins\\stats.bin", 
			std::ios::binary | std::ios::in);

		for (int i = 0; i < stats.size(); ++i)
			for (int j = 0; j < 6; ++j)
				file.read((char*)&stats[i][j], sizeof(int));

		file.close();
		return stats;
	}();

	return Stats(
		stats[no - 1][0],
		stats[no - 1][1],
		stats[no - 1][2],
		stats[no - 1][3],
		stats[no - 1][4],
		stats[no - 1][5]
	);
}

Types::Type Pokemon::getTypeByPokedexEntry(const int no, const int typeNo) {
	static std::vector<int[2]> types = []() {
		std::vector<int[2]> types(maxPokedexEntry);
		std::ifstream file(resourcesFolder + "Bins\\types.bin",
			std::ios::binary | std::ios::in);

		for (int i = 0; i < types.size(); ++i)
			for (int j = 0; j < 2; ++j)
				file.read((char*)&types[i][j], sizeof(int));

		file.close();
		return types;
	}();

	return (Types::Type)types[no - 1][typeNo - 1];
}

std::pair<double, double> Pokemon::getMeasurementsByPokedexEntry(const int no) {
	static std::vector<float[2]> measurements = []() {
		std::vector<float[2]> measurements(maxPokedexEntry);
		std::ifstream file(resourcesFolder + "Bins\\measurement.bin",
			std::ios::binary | std::ios::in);

		for (int i = 0; i < measurements.size(); ++i)
			for (int j = 0; j < 2; ++j)
				file.read((char*)&measurements[i][j], sizeof(float));

		file.close();
		return measurements;
	}();

	return std::make_pair(
		(double)measurements[no - 1][0], 
		(double)measurements[no - 1][1]
	);
}

Pokemon::Genders::Gender Pokemon::getGenderByPokedexEntry(const int no) {
	static std::pair< std::vector<int> , std::vector< std::pair<int, int> > > genders = []() {
		std::pair< std::vector<int>, std::vector< std::pair<int, int> > > genders;
		genders.first.resize(maxPokedexEntry);
		std::ifstream file(resourcesFolder + "Bins\\gender.bin",
			std::ios::binary | std::ios::in);

		int femaleRatio, maleRatio, count, entry;
		while (file.read((char*)&femaleRatio, sizeof(int))) {
			file.read((char*)&maleRatio, sizeof(int));

			genders.second.push_back(std::make_pair(femaleRatio, maleRatio));

			file.read((char*)&count, sizeof(int));
			for (int i = 0; i < count; ++i) {
				file.read((char*)&entry, sizeof(int));
				genders.first[entry - 1] = (int)genders.second.size();
			}
		}		

		file.close();
		return genders;
	}();

	const std::pair<int, int> ratio = genders.second[genders.first[no - 1] - 1];

	if (ratio.first == 0 && ratio.second == 0) {
		return Genders::none;
	}

	return random(1, ratio.first + ratio.second) <= ratio.first 
		? Genders::female : Genders::male;
}

void Pokemon::receiveDamage(int damage) {
	currentStats.hp -= damage;
	if (currentStats.hp <= 0) {
		if (getWillToFight() >= random(0.0, 1.0))
			currentStats.hp = (int)(random(0.01, 0.2) * maxStats.hp);
		else
			currentStats.hp = 0;
	}
}

void Pokemon::receiveDamage(int damage, Attack* move, Pokemon* attacker) {
	double pokemonPower = (attacker->getHappiness() * 200 + 10) / 250;
	double finalDmg = move->category == Categories::physical ?
		(double)attacker->currentStats.attack / currentStats.defense :
		(double)attacker->currentStats.spattack / currentStats.spdefense;

	double stab = attacker->type1 == move->type ||
		attacker->type2 == move->type ? 1.5 : 1;
	double type = getTypeAdvantageOverPokemon(move->type,
		type1, type2);
	double critical = 1;// move->criticalChance >= random(0.0, 1.0) ? 2 : 1;
	double modifier = stab * type * critical * random(0.85, 1.0);
	finalDmg = (finalDmg * pokemonPower * damage + 2) * modifier;

	auto console = &((TrainerBattleScene*)Scene::getInstance())->console;

	if (type == 0) {
		console->write("It does not affect " + nickname + " at all.");
	}
	else {
		if (type > 1)
			console->write("It is super effective!");
		if (type < 1)
			console->write("It is not very effective...");
	}

	if (critical == 2) {
		console->write("A critical hit!");
	}
	
	receiveDamage(finalDmg);
}
