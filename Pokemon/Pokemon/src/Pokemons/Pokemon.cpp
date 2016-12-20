#include "stdafx.h"
#include "Pokemons\Pokemon.h"

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

	// TODO: test this!
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
	: frozen(0), asleep(0), poisoned(0),
	burnt(0), paralyzed(0), confused(0),
	isSick(false), hunger(0), repugnance(0),
	sanity(random(0.0, 0.6)) {}

Pokemon::Stats::Stats(int SPD, int HP, int ATK, int DEF, int SPATK, int SPDEF) 
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
}

Pokemon* Pokemon::getStarter(const int no) {
	Pokemon* pokemon = new Pokemon(no, "Laboratory", 1);
	pokemon->traits.obedience = 1;
	return pokemon;
}
