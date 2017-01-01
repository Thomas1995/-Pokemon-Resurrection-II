#pragma once

#include "stdafx.h"
#include "Pokemons\Pokemon.h"
#include <string>
#include <queue>

namespace Graphics {
	class Drawable {
	public:
		virtual void draw(sf::RenderWindow& window) = 0;
		virtual ~Drawable();
	};

	class WithSizePosition {
	protected:
		Size<float> size;
		Position<float> position;
	public:
		WithSizePosition(const Size<float> size, const Position<float> pos);
		virtual ~WithSizePosition() = 0;
	};

	class Clickable : public WithSizePosition {
	public:
		Clickable(const Size<float> size, const Position<float> pos);
		void checkIfClicked(
			sf::RenderWindow& window,
			void(*onButtonPressed)(const std::vector<void*>&) = nullptr,
			const std::vector<void*>& args = std::vector<void*>()
		);
		virtual ~Clickable() = 0;
	};

	class Image : public Drawable {
		sf::Texture* texture;
	protected:
		sf::Sprite* sprite;
	public:
		Image(const std::string path, 
			const Size<float> size = Size<float>(), 
			const Position<float> pos = Position<float>());

		void resize(const Size<float> size);
		void moveTo(const Position<float> pos);

		void draw(sf::RenderWindow& window);
		~Image();
	};

	class Background : public Image {
	public:
		Background(std::string name,
			const Size<float> size = Size<float>(),
			const Position<float> pos = Position<float>());
	};
	
	class PokemonInBattle : public Image {
	public:
		PokemonInBattle(Pokemon* pokemon, std::string type,
			const Size<float> size = Size<float>(),
			const Position<float> pos = Position<float>());
	};

	class PokemonInBattleReserve : public PokemonInBattle, public Clickable {
	public:
		PokemonInBattleReserve(Pokemon* pokemon, std::string type,
			const Size<float> size = Size<float>(),
			const Position<float> pos = Position<float>());
		void darken();
	};

	class Console : public Drawable, public WithSizePosition {
		std::string textFinal, textInProgress;
		std::queue<std::string> messages;
		sf::Color consoleColor;
		sf::Color textColor;
		int messageCount;
	public:
		Console(const Size<float> size, const Position<float> pos,
			const sf::Color consoleColor, const sf::Color textColor);
		void draw(sf::RenderWindow& window);
		void clear();
		void write(std::string msg = " ");
	};

	class Bar : public Drawable {
		sf::Vertex rect[4];
	public:
		Bar(const Position<float> topleft, const Position<float> bottomright,
			sf::Color topColor, sf::Color bottomColor);
		void draw(sf::RenderWindow& window);
	};

	class Button : public Drawable, public Clickable {
		std::string text;
		sf::Color color;
	public:
		Button(std::string text, Size<float> size, Position<float> pos, sf::Color color);
		void draw(sf::RenderWindow& window);
	};
}