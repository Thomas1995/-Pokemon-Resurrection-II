#pragma once

#include "stdafx.h"
#include "Pokemons\Pokemon.h"
#include <string>

class Image {
	sf::Texture* texture;
	sf::Sprite* sprite;
public:
	Image(const std::string path);
	Image(const std::string path, const Size<float> scaleSize);
	Image(const std::string path, const Size<float> scaleSize, const Position<float> pos);

	void resize(const Size<float> size);
	void scale(const Size<float> scaleSize);
	void moveTo(const Position<float> pos);
	void addOpacity();

	sf::Sprite operator* ();

	~Image();
};

struct Images {
	static std::string PokemonFront(Pokemon* pokemon);
	static std::string PokemonBack(Pokemon* pokemon);

	static std::string Background(std::string backgroundName);
};