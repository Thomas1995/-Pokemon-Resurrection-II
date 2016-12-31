#pragma once

struct Types {
	enum Type {
		none,
		normal,
		fire,
		water,
		electric,
		grass,
		ice,
		fighting,
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
};

double getTypeAdvantage(Types::Type attackType, Types::Type defenderType);
double getTypeAdvantageOverPokemon(Types::Type attackType, 
	Types::Type defenderType1, Types::Type defenderType2);