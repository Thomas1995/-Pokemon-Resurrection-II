#pragma once

class Game {
	static Game* instance;
	sf::RenderWindow window;

public:
	const Size size;

public:
	static Game* initialize(const Size screenSize, const bool fullscreen = false);
	static Game* getInstance();
	void start();

private:
	Game(const Size screenSize, const int style);
	~Game();

	void onFrameUpdate();
	void Game::onEvent(const sf::Event& event);
};