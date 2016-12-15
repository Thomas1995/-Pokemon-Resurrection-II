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
		normalize(BMI / 2.0, 0, 50)
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

	// a hothead gets angry easily
	if (personality == Personalities::hothead)
		anger += 0.2;

	return normalize(anger);
}