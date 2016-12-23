#pragma once

#include "..\Pokemons\Trainer.h"

class Scene {
	static Scene* instance;

public:
	virtual void Draw(sf::RenderWindow& window) = 0;

	static void updateInstance(Scene* inst);
	static void releaseInstance();
	static Scene* getInstance();
};

class TrainerBattleScene : public Scene {
public:
	const Trainer *trainerA, *trainerB;

	TrainerBattleScene(const Trainer* A, const Trainer* B);

	void Draw(sf::RenderWindow& window);
};