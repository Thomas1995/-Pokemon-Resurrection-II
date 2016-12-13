#include "stdafx.h"
#include "assert.h"
#include "Game\Game.h"

Game* Game::instance = nullptr;

Game* Game::initialize(const Size screenSize, const bool fullscreen) {
	assert(nullptr == instance);
	instance = new Game(screenSize, 
		fullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar | sf::Style::Close);
	return instance;
}

Game* Game::getInstance() {
	assert(nullptr != instance);
	return instance;
}

void Game::start() {
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event))
			onEvent(event);

		window.clear();
		onFrameUpdate();
		window.display();
	}
}

Game::Game(const Size screenSize, const int style) : 
	size(screenSize), 
	window(sf::VideoMode(screenSize.width, screenSize.height), 
		"Pokemon Resurrection II", style) {
	window.setFramerateLimit(60);

	// TODO: load an icon
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

}