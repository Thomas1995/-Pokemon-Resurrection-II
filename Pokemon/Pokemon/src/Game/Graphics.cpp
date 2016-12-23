#include "stdafx.h"
#include "Game\Graphics.h"

Image::Image(const std::string path) {
	texture = new sf::Texture;
	if (!texture->loadFromFile(path)) {
		// do something
	}
	sprite = new sf::Sprite(*texture);
}

Image::Image(const std::string path, const Size<float> scaleSize)
	: Image(path) {
	scale(scaleSize);
}

Image::Image(const std::string path, const Size<float> scaleSize,
	const Position<float> pos) : Image(path, scaleSize) {
	moveTo(pos);
}

void Image::scale(const Size<float> scaleSize) {
	sprite->setScale(sf::Vector2f(scaleSize.width, scaleSize.height));
}

void Image::resize(const Size<float> size) {
	int oldWidth = sprite->getTextureRect().width;
	int oldHeight = sprite->getTextureRect().height;
	scale(Size<float>(size.width / oldWidth, size.height / oldHeight));
}

void Image::moveTo(const Position<float> pos) {
	sprite->setPosition(sf::Vector2f(pos.X, pos.Y));
}

void Image::addOpacity() {
	sprite->setColor(sf::Color::Color(50, 50, 50));
}

sf::Sprite Image::operator* () {
	return *sprite;
}

Image::~Image() {
	delete texture;
	delete sprite;
}

std::string Images::PokemonFront(Pokemon * pokemon) {
	return resourcesFolder + "Sprites\\Pokemon\\front\\" +
		(pokemon->shiny ? "shiny\\" : "normal\\") +
		std::to_string(pokemon->pokedexEntry) + ".png";
}

std::string Images::PokemonBack(Pokemon * pokemon) {
	return resourcesFolder + "Sprites\\Pokemon\\back\\" +
		(pokemon->shiny ? "shiny\\" : "normal\\") +
		std::to_string(pokemon->pokedexEntry) + ".png";
}

std::string Images::Background(std::string backgroundName) {
	return resourcesFolder + "Backgrounds\\" + backgroundName + ".png";
}
