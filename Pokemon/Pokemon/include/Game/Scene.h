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

	/*
		A v.s. B
	*/
	TrainerBattleScene(Trainer* A, Trainer* B);

	void draw(sf::RenderWindow& window);
	void logic();
	void switchOut(Trainer* trainer, Pokemon* chosenPokemon);
private:
	Graphics::Console console;

	void turn(Trainer* trainer);
};