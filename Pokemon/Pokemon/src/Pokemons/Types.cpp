#include "stdafx.h"
#include "Pokemons\Types.h"
#include <fstream>

double getTypeAdvantage(Types::Type attackType, Types::Type defenderType) {
	static std::vector<float[18]> effectivenessTable = []() {
		std::vector<float[18]> effectivenessTable(18);
		std::ifstream file(resourcesFolder + "Bins\\effectiveness.bin",
			std::ios::binary | std::ios::in);

		for (int i = 0; i < effectivenessTable.size(); ++i)
			for (int j = 0; j < effectivenessTable.size(); ++j)
				file.read((char*)&effectivenessTable[i][j], sizeof(float));

		file.close();
		return effectivenessTable;
	}();

	return (double)effectivenessTable[attackType - 1][defenderType - 1];
}

double getTypeAdvantageOverPokemon(Types::Type attackType,
	Types::Type defenderType1, Types::Type defenderType2) {
	return getTypeAdvantage(attackType, defenderType1) *
		(defenderType2 == Types::none ? 1 :
		getTypeAdvantage(attackType, defenderType2));
}