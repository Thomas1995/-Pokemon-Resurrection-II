#include "stdafx.h"
#include "Game\Graphics.h"
#include "Game\Scene.h"
using namespace Graphics;

Drawable::~Drawable() {}

Graphics::WithSizePosition::WithSizePosition(const Size<float> size, const Position<float> pos)
	: size(size), position(pos) {}

WithSizePosition::~WithSizePosition() {}

Image::Image(const std::string path, const Size<float> size, const Position<float> pos) {
	texture = new sf::Texture;
	if (!texture->loadFromFile(path)) {
		// do something
	}
	sprite = new sf::Sprite(*texture);

	resize(size);
	moveTo(pos);
}

void Image::resize(const Size<float> size) {
	int oldWidth = sprite->getTextureRect().width;
	int oldHeight = sprite->getTextureRect().height;
	sprite->setScale(sf::Vector2f(size.width / oldWidth, size.height / oldHeight));
}

void Image::moveTo(const Position<float> pos) {
	sprite->setPosition(sf::Vector2f(pos.X, pos.Y));
}

void Image::draw(sf::RenderWindow& window) {
	window.draw(*sprite);
}

Image::~Image() {
	delete texture;
	delete sprite;
}

Background::Background(std::string name, const Size<float> size, const Position<float> pos) 
	: Image(resourcesFolder + "Backgrounds\\" + name + ".png", size, pos) {}

PokemonInBattle::PokemonInBattle(Pokemon * pokemon, std::string type, 
	const Size<float> size, const Position<float> pos) 
	: Image(resourcesFolder + "Sprites\\Pokemon\\" + type + 
		"\\" + (pokemon->shiny ? "shiny\\" : "normal\\") +
		std::to_string(pokemon->pokedexEntry) + ".png",
		size, pos) {}

PokemonInBattleReserve::PokemonInBattleReserve(Pokemon* pokemon, std::string type,
	const Size<float> size, const Position<float> pos) 
	: PokemonInBattle(pokemon, type, size, pos), 
	Clickable(size, pos) {}

void PokemonInBattleReserve::darken() {
	sprite->setColor(sf::Color::Color(50, 50, 50));
}

Console::Console(const Size<float> size, const Position<float> pos,
	const sf::Color consoleColor, const sf::Color textColor)
	: WithSizePosition(size, pos), consoleColor(consoleColor), textColor(textColor) {}

void Console::draw(sf::RenderWindow& window) {
	sf::RectangleShape console(sf::Vector2f(size.width, size.height));
	console.setPosition(sf::Vector2f(position.X, position.Y));
	console.setFillColor(consoleColor);
	window.draw(console);

	if (textInProgress.size() < textFinal.size()) {
		textInProgress.push_back(textFinal[textInProgress.size()]);

		if(textInProgress.size() == textFinal.size())
			--Scene::userActionsDisabledCount;
	}
	else {
		// there are still messages to be written
		if (!messages.empty()) {

			++Scene::userActionsDisabledCount;
			textFinal += messages.front() + "\n\n";
			messages.pop();

			// delete first message if message count is 12
			if (messageCount == 12) {
				// find first new line
				size_t pos = textFinal.find_first_of('\n');

				// skip all new lines
				while (textFinal[pos] == '\n')
					++pos;

				// get the new string, with the first entry erased
				textFinal = textFinal.substr(pos);
				textInProgress = textInProgress.substr(pos);
			}
			else {
				++messageCount;
			}

		}
	}

	sf::Font font;
	font.loadFromFile(resourcesFolder + "Fonts\\pkmndp.ttf");

	sf::Text consoleText(textInProgress, font, 12);
	consoleText.setFillColor(sf::Color::White);
	consoleText.setPosition(console.getPosition());
	window.draw(consoleText);
}

void Console::clear() {
	messageCount = 0;
	textFinal.clear();
	textInProgress.clear();
}

void Console::write(std::string msg) {
	messages.push(msg);
}

Bar::Bar(const Position<float> topleft, const Position<float> bottomright, 
	sf::Color topColor, sf::Color bottomColor) {
	rect[0] = sf::Vertex(sf::Vector2f(topleft.X, topleft.Y), topColor);
	rect[1] = sf::Vertex(sf::Vector2f(bottomright.X, topleft.Y), topColor);
	rect[2] = sf::Vertex(sf::Vector2f(bottomright.X, bottomright.Y), bottomColor);
	rect[3] = sf::Vertex(sf::Vector2f(topleft.X, bottomright.Y), bottomColor);
}

void Bar::draw(sf::RenderWindow& window) {
	window.draw(rect, 4, sf::Quads);
}

Clickable::Clickable(const Size<float> size, const Position<float> pos) 
	: WithSizePosition(size, pos) {}

Clickable::~Clickable() {}

void Clickable::checkIfClicked(
	sf::RenderWindow& window,
	void(*onButtonPressed)(const std::vector<void*>&), 
	const std::vector<void*>& args) {

	if (Scene::userActionsDisabledCount > 0)
		return;

	if (onButtonPressed != nullptr &&
		sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);

		if (isBetween((float)mousePos.x, position.X, position.X + size.width) &&
			isBetween((float)mousePos.y, position.Y, position.Y + size.height)) {
			onButtonPressed(args);
		}
	}
}

Button::Button(std::string text, Size<float> size, Position<float> pos, sf::Color color) 
	: Clickable(size, pos), text(text), color(color) {}

void Button::draw(sf::RenderWindow& window) {
	sf::RectangleShape button(sf::Vector2f(size.width, size.height));
	button.setPosition(sf::Vector2f(position.X, position.Y));
	button.setOutlineThickness(1);
	button.setOutlineColor(sf::Color::Black);
	button.setFillColor(color);
	window.draw(button);

	sf::Font font;
	font.loadFromFile(resourcesFolder + "Fonts\\pkmndp.ttf");

	int fontSize = 12;
	int textLen = (int)text.size();

	// if text is big, split in two
	if (text.size() >= 20) {
		textLen /= 2;
		while (text[textLen] != ' ' && textLen < text.size())
			++textLen;
		text[textLen--] = '\n';
	}

	sf::Text buttonText(text, font, fontSize);
	buttonText.setFillColor(sf::Color::Black);
	buttonText.setPosition(sf::Vector2f(
		(position.X + size.width / 2 - fontSize / 4.5f * textLen),
		position.Y)
	);
	window.draw(buttonText);
}