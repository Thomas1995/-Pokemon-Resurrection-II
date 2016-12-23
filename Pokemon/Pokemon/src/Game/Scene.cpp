#include "stdafx.h"
#include "Game\Game.h"
#include "Game\Scene.h"
#include "Game\Graphics.h"
#include <assert.h>

Scene* Scene::instance = nullptr;

Scene* Scene::getInstance() {
	return instance;
}

void Scene::updateInstance(Scene * inst) {
	if (instance)
		delete instance;

	instance = inst;
}

void Scene::releaseInstance() {
	delete instance;
}

TrainerBattleScene::TrainerBattleScene(const Trainer* A, const Trainer* B)
	: trainerA(A), trainerB(B) {
	assert(!A->pokemonInTeam.empty() && !B->pokemonInTeam.empty());
}

void TrainerBattleScene::Draw(sf::RenderWindow& window) {
	const Size<unsigned int> screenSize = Game::getInstance()->size;
	const Size<float> unitSize(screenSize.width / 800.0f, screenSize.height / 600.0f);

	Image background(Images::Background("battlebackground"));
	background.resize(Size<float>(screenSize.width, screenSize.height / 2.0f));
	window.draw(*background);

	// draw current battling pokemon

	Position<float> 
		posPokemonA(unitSize.width * 170, unitSize.height * 210),
		posPokemonB(unitSize.width * 540, unitSize.height * 90);

	Image pokemonA(Images::PokemonBack(trainerA->pokemonInTeam[0]),
		unitSize, posPokemonA);

	Image pokemonB(Images::PokemonFront(trainerB->pokemonInTeam[0]),
		unitSize, posPokemonB);

	window.draw(*pokemonA);
	window.draw(*pokemonB);

	// TODO: change this thing with vertexes

	// draw attack bar

	sf::Vertex r[] =
	{
		sf::Vertex(sf::Vector2f(610, 300), sf::Color::Color(0, 0, 0)),
		sf::Vertex(sf::Vector2f(800, 300), sf::Color::Color(222, 184, 135)),
		sf::Vertex(sf::Vector2f(800, 600), sf::Color::Color(222, 184, 135)),
		sf::Vertex(sf::Vector2f(610, 600), sf::Color::Color(0, 0, 0)),
	};

	window.draw(r, 4, sf::Quads);

	// draw available pokemon bar

	sf::Vertex r2[] =
	{
		sf::Vertex(sf::Vector2f(0, 500), sf::Color::Color(222, 184, 135)),
		sf::Vertex(sf::Vector2f(620, 500), sf::Color::Color(0, 0, 0)),
		sf::Vertex(sf::Vector2f(620, 600), sf::Color::Color(0, 0, 0)),
		sf::Vertex(sf::Vector2f(0, 600), sf::Color::Color(222, 184, 135)),
	};

	window.draw(r2, 4, sf::Quads);

	/*sf::RectangleShape rectangle(sf::Vector2f(unitSize.width * 620, unitSize.height * 100));
	rectangle.setPosition(sf::Vector2f(0, unitSize.height * 500));
	rectangle.setFillColor(sf::Color::Color(222, 184, 135));
	window.draw(rectangle);*/

	for (int i = 0; i < trainerA->pokemonInTeam.size(); ++i) {
		Image pokemonInTeam(Images::PokemonFront(trainerA->pokemonInTeam[i]), 
			unitSize, Position<float>(unitSize.width * (10 + 100 * i), unitSize.height * 502));
		
		if (trainerA->pokemonInTeam[i]->isFainted())
			pokemonInTeam.addOpacity();

		window.draw(*pokemonInTeam);
	}
}