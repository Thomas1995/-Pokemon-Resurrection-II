#pragma once

#include "stdafx.h"

class Game {
	static Game* instance;
	sf::RenderWindow window;

public:
	const Size<unsigned int> size;

	static Game* initialize(const Size<unsigned int> screenSize, const bool fullscreen = false);
	static Game* getInstance();
	void start();

private:
	Game(const Size<unsigned int> screenSize, const int style);
	~Game();

	void onFrameUpdate();
	void Game::onEvent(const sf::Event& event);
};