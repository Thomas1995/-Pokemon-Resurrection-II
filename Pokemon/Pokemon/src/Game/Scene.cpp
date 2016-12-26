#include "stdafx.h"
#include "Game\Game.h"
#include "Game\Scene.h"
#include <assert.h>

Scene* Scene::instance = nullptr;
std::atomic<int> Scene::userActionsDisabledCount = 0;

Scene::Scene() {
	userActionsDisabledCount = 0;
}

Scene* Scene::getInstance() {
	return instance;
}

void Scene::updateInstance(Scene* inst) {
	if (instance)
		delete instance;

	instance = inst;
}

void Scene::releaseInstance() {
	delete instance;
}

TrainerBattleScene::TrainerBattleScene(Trainer* A, Trainer* B)
	: trainerA(A), trainerB(B),
	console(Size<float>(560 * 1, 200 * 1),
		Position<float>(0, 300 * 1),
		sf::Color::Black, sf::Color::White) {
	assert(!A->pokemonInTeam.empty() && !B->pokemonInTeam.empty());
}

void TrainerBattleScene::draw(sf::RenderWindow& window) {
	static const Size<unsigned int> screenSize = Game::getInstance()->size;
	static const Size<float> unitSize(screenSize.width / 800.0f, screenSize.height / 600.0f);

	static Graphics::Background background("battlebackground", 
		Size<float>((float)screenSize.width, screenSize.height / 2.0f));
	background.draw(window);

	console.draw(window);

	// draw current battling pokemon

	if(pokemonA == NULL)
		pokemonA = trainerA->pokemonInTeam[0];
	pokemonB = trainerB->pokemonInTeam[0];

	Position<float> 
		posPokemonBack(unitSize.width * 170, unitSize.height * 210),
		posPokemonFront(unitSize.width * 540, unitSize.height * 90);
	Size<float> pokemonSpriteSize(96 * unitSize.width, 96 * unitSize.height);

	Graphics::PokemonInBattle pokemonBack(pokemonA, "back", pokemonSpriteSize, posPokemonBack);
	Graphics::PokemonInBattle pokemonFront(pokemonB, "front", pokemonSpriteSize, posPokemonFront);
	pokemonBack.draw(window);
	pokemonFront.draw(window);

	// draw attack bar

	static Graphics::Bar attackBar(
		Position<float>(560 * unitSize.width, 300 * unitSize.height),
		Position<float>(800 * unitSize.width, 500 * unitSize.height),
		sf::Color::Color(222, 184, 135), sf::Color::Color(0, 0, 0));

	attackBar.draw(window);

	std::vector<void*> attackBtnFctArgs(1);
	attackBtnFctArgs[0] = (void*)&console;
	Size<float> attackBtnSize(95 * unitSize.width, 22 * unitSize.height);
	sf::Color attackBtnColor(222, 184, 135);

	for (int i = 0; i < 8; ++i) {
		Position<float> attackBtnPos(
			(575.0f + 115 * (i / 4)) * unitSize.width, 
			(320.0f + (i % 4) * 38) * unitSize.height
		);

		Graphics::Button button(
			pokemonA->moves.size() > i ? pokemonA->moves[i].name : "-", 
			attackBtnSize, attackBtnPos, attackBtnColor
		);

		button.draw(window);

		if (pokemonA->moves.size() > i) {
			button.checkIfClicked(window, 
				[](const std::vector<void*>& args) {
				Graphics::Console* consolePtr = (Graphics::Console*)args[0];
				consolePtr->write("abc");
			}, attackBtnFctArgs);
		}
	}

	// draw available pokemon bar

	static Graphics::Bar availablePokemonBar(
		Position<float>(0, 500 * unitSize.height),
		Position<float>(800 * unitSize.width, 600 * unitSize.height),
		sf::Color::Color(0, 0, 0), sf::Color::Color(222, 184, 135));

	availablePokemonBar.draw(window);

	static std::vector<void*> pokemonBarFctArgs(2);
	pokemonBarFctArgs[0] = (void*)trainerA;

	for (int i = 0; i < trainerA->pokemonInTeam.size(); ++i) {
		Graphics::PokemonInBattleReserve pokemonInTeam(trainerA->pokemonInTeam[i], "front",
			pokemonSpriteSize, Position<float>(unitSize.width * 100 * (i + 1), unitSize.height * 502));
		
		if (trainerA->pokemonInTeam[i]->isFainted())
			pokemonInTeam.darken();

		pokemonInTeam.draw(window);

		pokemonBarFctArgs[1] = (void*)i;
		if (!trainerA->pokemonInTeam[i]->isFainted()) {
			pokemonInTeam.checkIfClicked(window, [](const std::vector<void*>& args) {
				//Graphics::Console* consolePtr = (Graphics::Console*)args[0];
				//++Scene::userActionsDisabledCount;
				//consolePtr->write("Ana are mere. ");
				MainTrainer* trainer = (MainTrainer*)args[0];
				trainer->notify(args);
			}, pokemonBarFctArgs);
		}
	}
}

void TrainerBattleScene::logic() {
	trainerA->takeTurnInBattle();
	++Scene::userActionsDisabledCount;
	trainerB->takeTurnInBattle();
	--Scene::userActionsDisabledCount;
}

void TrainerBattleScene::switchOut(Trainer* trainer, Pokemon* chosenPokemon) {
	if (trainer == trainerA) {
		pokemonA = chosenPokemon;
	}
	else {
		pokemonB = chosenPokemon;
	}

	console.write(trainer->name + " has switched out to " + chosenPokemon->nickname + ".");
}

void TrainerBattleScene::turn(Trainer* trainer) {}