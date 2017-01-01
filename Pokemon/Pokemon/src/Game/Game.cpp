#include "stdafx.h"
#include <assert.h>
#include "Game\Game.h"
#include "Game\Scene.h"
#include <thread>

Game* Game::instance = nullptr;

Game* Game::initialize(const Size<unsigned int> screenSize, const bool fullscreen) {
	assert(nullptr == instance);
	instance = new Game(screenSize, 
		fullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar | sf::Style::Close);
	return instance;
}

Game* Game::getInstance() {
	assert(nullptr != instance);
	return instance;
}

void Game::initializeScene() {
	Pokemon* pokemon1 = Pokemon::getStarter(35);
	Pokemon* pokemon2 = Pokemon::getStarter(1);
	//pokemon1->moves[0].pp = 1;
	//pokemon1->currentStats.hp = 0;
	Trainer *me = new MainTrainer("Thomas"), *enemy = new Trainer("Cristian");
	me->pokemonInTeam.push_back(pokemon1);
	me->pokemonInTeam.push_back(new Pokemon(122));
	me->pokemonInTeam.push_back(new Pokemon(123));
	me->pokemonInTeam.push_back(new Pokemon(124));
	me->pokemonInTeam.push_back(new Pokemon(25));
	me->pokemonInTeam.push_back(new Pokemon(120));
	enemy->pokemonInTeam.push_back(pokemon2);
	enemy->pokemonInTeam.push_back(new Pokemon(5));

	for (auto &pokemon : me->pokemonInTeam)
		pokemon->moves.push_back(Attack::get("Tackle"));

	for (auto &pokemon : enemy->pokemonInTeam)
		pokemon->moves.push_back(Attack::get("Pound"));

	Scene::updateInstance(new TrainerBattleScene(me, enemy));

	auto inst = Scene::getInstance();
}

void Game::gameLogic() {
	while (true) {
		Scene::getInstance()->logic();
	}
}

void Game::start() {
	initializeScene();

	std::thread t(&Game::gameLogic, this);
	t.detach();

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event))
			onEvent(event);

		window.clear();
		onFrameUpdate();
		window.display();
	}
}

Game::Game(const Size<unsigned int> screenSize, const int style) :
	size(screenSize), 
	window(sf::VideoMode(screenSize.width, screenSize.height), 
		"Pokemon Resurrection II", style) {
	window.setFramerateLimit(30);

	// TODO: load an icon
	// hint: window.SetIcon(image.GetWidth(), image.GetHeight(), image.GetPixelsPtr());
	sf::Uint8 ico[32 * 32 * 4 + 1];
	window.setIcon(32, 32, ico);
}

Game::~Game() {
	delete instance;
}

void Game::onEvent(const sf::Event& event) {
	if (event.type == sf::Event::Closed)
		window.close();
}

void Game::onFrameUpdate() {
	Scene::getInstance()->draw(window);
}