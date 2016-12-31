#pragma once

#include "..\Pokemons\Trainer.h"
#include "Game\Graphics.h"
#include <atomic>

class Scene {
	static Scene* instance;

public:
	static std::atomic<int> userActionsDisabledCount;
	Scene();

	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void logic() = 0;

	static void updateInstance(Scene* inst);
	static void releaseInstance();
	static Scene* getInstance();
};

class TrainerBattleScene : public Scene {
public:
	Trainer *trainerA, *trainerB;
	Pokemon *pokemonA, *pokemonB;
	Graphics::Console console;

	/*
		A v.s. B
	*/
	TrainerBattleScene(Trainer* A, Trainer* B);

	void onBattleStart();
	void onBattleFinish();
	void draw(sf::RenderWindow& window);
	void logic();
	void switchOut(Trainer* trainer, Pokemon* chosenPokemon);
	void pokemonWantsToUseMove(Pokemon* attacker, Pokemon* target, Attack* move);
	void pokemonUsesMove(Pokemon* attacker, Pokemon* target, Attack* move);

private:
	struct InternalAtk {
		Pokemon *attacker, *target;
		Attack *move;

		InternalAtk(Pokemon* attacker, Pokemon* target, Attack* move);
	};
	std::vector<InternalAtk> atk;

	void restorePokemonToDefault(Trainer* trainer);
};