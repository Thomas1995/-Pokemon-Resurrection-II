#include "stdafx.h"
#include "Game\Game.h"
#include "Game\Scene.h"
#include "Pokemons\Types.h"
#include <assert.h>

TrainerBattleScene::TrainerBattleScene(Trainer* A, Trainer* B)
	: trainerA(A), trainerB(B),
	console(Size<float>(560 * 1, 200 * 1),
		Position<float>(0, 300 * 1),
		sf::Color::Black, sf::Color::White) {
	assert(!A->pokemonInTeam.empty() && !B->pokemonInTeam.empty());
	onBattleStart();
}

void TrainerBattleScene::draw(sf::RenderWindow& window) {
	static const Size<unsigned int> screenSize = Game::getInstance()->size;
	static const Size<float> unitSize(screenSize.width / 800.0f, screenSize.height / 600.0f);

	static Graphics::Background background("battlebackground",
		Size<float>((float)screenSize.width, screenSize.height / 2.0f));
	background.draw(window);

	console.draw(window);

	static int animationCounterLimits[] = { 0, 18, 18, 42, 42, 40 };

	// if an animation is in progress, increment the counter
	if (animationIndex)
		if (animationCounter++ == animationCounterLimits[animationIndex]) {
			animationIndex = 0;
			animationCounter = 0;
		}

	// draw current battling pokemon

	static Position<float>
		posPokemonBack(unitSize.width * 170, unitSize.height * 210),
		posPokemonFront(unitSize.width * 540, unitSize.height * 90);
	static const Size<float> pokemonSpriteSize(96 * unitSize.width, 96 * unitSize.height);

	// animations

	int attackAnimationOffset = (animationCounter <=
		animationCounterLimits[animationIndex] / 2 ? -4 : 4) * unitSize.height;

	if (animationIndex == 1)
		posPokemonBack.Y += attackAnimationOffset;

	if (animationIndex == 2)
		posPokemonFront.Y += attackAnimationOffset;

	if (animationIndex == 3) {
		float x = animationCounter * 4;
		float y = (0.019f * x * x - 3.29f * x + 290);
		Position<float> pokeballPos(x * unitSize.width, y * unitSize.height);
		Graphics::Image pokeball(resourcesFolder + "Sprites\\Pokeball\\pokeball.png", Size<float>(24, 24), pokeballPos);
		pokeball.draw(window);
	}

	if (animationIndex == 4) {
		float x = 800 - animationCounter * 4;
		float y = (0.019f * x * x - 27.11f * x + 9698);
		Position<float> pokeballPos(x * unitSize.width, y * unitSize.height);
		Graphics::Image pokeball(resourcesFolder + "Sprites\\Pokeball\\pokeball.png", Size<float>(24, 24), pokeballPos);
		pokeball.draw(window);
	}

	if (pokemonA != nullptr) {
		Graphics::PokemonInBattle pokemonBack(pokemonA, "back", pokemonSpriteSize, posPokemonBack);
		pokemonBack.draw(window);
	}

	if (pokemonB != nullptr) {
		Graphics::PokemonInBattle pokemonFront(pokemonB, "front", pokemonSpriteSize, posPokemonFront);
		pokemonFront.draw(window);
	}

	// draw attack bar

	static Graphics::Bar attackBar(
		Position<float>(560 * unitSize.width, 300 * unitSize.height),
		Position<float>(800 * unitSize.width, 500 * unitSize.height),
		sf::Color::Color(222, 184, 135), sf::Color::Color(0, 0, 0));

	attackBar.draw(window);

	static std::vector<void*> attackBtnFctArgs(5);

	// on 0 there is the pointer to trainer to be used internally in the lambda function
	attackBtnFctArgs[0] = (void*)trainerA;
	// on 2 there is the index of the used pokemon
	attackBtnFctArgs[2] = (void*)pokemonA;
	// on 3 there is a pointer to the target
	attackBtnFctArgs[3] = (void*)pokemonB;

	Size<float> attackBtnSize(95 * unitSize.width, 22 * unitSize.height);
	sf::Color attackBtnColor(222, 184, 135);

	for (int i = 0; i < 8; ++i) {
		Position<float> attackBtnPos(
			(575.0f + 115 * (i / 4)) * unitSize.width,
			(320.0f + (i % 4) * 38) * unitSize.height
		);

		Graphics::Button button(
			pokemonA != nullptr && pokemonA->moves.size() > i ? pokemonA->moves[i].name : "-",
			attackBtnSize, attackBtnPos, attackBtnColor
		);

		button.draw(window);

		if (pokemonA != nullptr && pokemonA->moves.size() > i && animationIndex == 0) {
			if (pokemonA->moves[i].isUsable()) {

				// on 4 there is a pointer to the attack used
				attackBtnFctArgs[4] = (void*)&pokemonA->moves[i];

				button.checkIfClicked(window,
					[](const std::vector<void*>& args) {
					// on 1 there is the type of action (1 = attack)
					attackBtnFctArgs[1] = (void*)1;
					MainTrainer* trainer = (MainTrainer*)args[0];
					trainer->notify(args);

				}, attackBtnFctArgs);

			}
			else {

				button.checkIfClicked(window,
					[](const std::vector<void*>& args) {
					TrainerBattleScene* scene = (TrainerBattleScene*)Scene::getInstance();
					scene->console.write("There is no PP left for this move.");

				});
			}
		}
	}

	// draw available pokemon bar

	static Graphics::Bar availablePokemonBar(
		Position<float>(0, 500 * unitSize.height),
		Position<float>(800 * unitSize.width, 600 * unitSize.height),
		sf::Color::Color(0, 0, 0), sf::Color::Color(222, 184, 135));

	availablePokemonBar.draw(window);

	static std::vector<void*> pokemonBarFctArgs(3);
	// on 0 there is the pointer to trainer to be used internally in the lambda function
	pokemonBarFctArgs[0] = (void*)trainerA;

	for (int i = 0; i < trainerA->pokemonInTeam.size(); ++i) {
		Graphics::PokemonInBattleReserve pokemonInTeam(trainerA->pokemonInTeam[i], "front",
			pokemonSpriteSize, Position<float>(unitSize.width * 100 * (i + 1), unitSize.height * 502));

		if (trainerA->pokemonInTeam[i]->isFainted())
			pokemonInTeam.darken();

		pokemonInTeam.draw(window);

		// on 2 there is the index of the used pokemon
		pokemonBarFctArgs[2] = (void*)i;
		if (!trainerA->pokemonInTeam[i]->isFainted() && trainerA->pokemonInTeam[i] != pokemonA 
			&& animationIndex == 0) {
			pokemonInTeam.checkIfClicked(window, [](const std::vector<void*>& args) {
				// on 1 there is the type of action (0 = change pokemon)
				pokemonBarFctArgs[1] = (void*)0;
				MainTrainer* trainer = (MainTrainer*)args[0];
				trainer->notify(args);

			}, pokemonBarFctArgs);
		}
	}
}

void TrainerBattleScene::choosePokemonIfNecessary(Trainer*& trainer, Pokemon*& pokemon) {
	if (pokemon == nullptr) {
		Pokemon* chosenPokemon = trainer->choosePokemonInBattle();

		animationIndex = trainer == trainerA ? 3 : 4;
		while (animationIndex);

		pokemon = chosenPokemon;
		console.write(trainer->name + " has chosen " + pokemon->nickname + ".");
		console.write();
	}
}

void TrainerBattleScene::logic() {
	atk.clear();

	choosePokemonIfNecessary(trainerA, pokemonA);

	++Scene::userActionsDisabledCount;
	choosePokemonIfNecessary(trainerB, pokemonB);
	--Scene::userActionsDisabledCount;

	trainerA->takeTurnInBattle();

	++Scene::userActionsDisabledCount;
	trainerB->takeTurnInBattle();

	if (atk.size() == 2)
		if (atk[0].attacker->currentStats.speed < atk[1].attacker->currentStats.speed)
			std::swap(atk[0], atk[1]);

	Pokemon* target;

	if (atk.size() >= 1 && !atk[0].attacker->isFainted()) {
		target = pokemonA;
		animationIndex = 2;

		if (atk[0].attacker == pokemonA) {
			target = pokemonB;
			animationIndex = 1;
		}

		while (animationIndex);
		pokemonUsesMove(atk[0].attacker, target, atk[0].move);
	}

	if (atk.size() == 2 && !atk[1].attacker->isFainted()) {
		target = pokemonA;
		animationIndex = 2;

		if (atk[1].attacker == pokemonA) {
			target = pokemonB;
			animationIndex = 1;
		}

		while (animationIndex);
		pokemonUsesMove(atk[1].attacker, target, atk[1].move);
	}

	checkIfPokemonFainted(pokemonA);
	checkIfPokemonFainted(pokemonB);

	if (trainerA->checkIfTeamDead() || trainerB->checkIfTeamDead()) {
		onBattleFinish();
	}

	--Scene::userActionsDisabledCount;
}

void TrainerBattleScene::onBattleStart() {}

void TrainerBattleScene::onBattleFinish() {
	Trainer* winner = trainerA;
	Trainer* loser = trainerB;

	if (trainerA->checkIfTeamDead())
		std::swap(winner, loser);

	console.write(winner->name + " has defeated " + loser->name + ".");

	restorePokemonToDefault(trainerA);
	restorePokemonToDefault(trainerB);
}

void TrainerBattleScene::switchOut(Trainer* trainer, Pokemon* chosenPokemon) {
	if (trainer == trainerA) {
		if (pokemonA->traits.obedience >= random(0.0, 1.0)) {
			pokemonA = nullptr;

			animationIndex = 3;
			while (animationIndex);

			pokemonA = chosenPokemon;
		}
		else {
			console.write(pokemonA->nickname + " refused to leave the battle.");
			console.write();
			return;
		}
	}
	else {
		pokemonA = nullptr;

		animationIndex = 4;
		while (animationIndex);

		pokemonB = chosenPokemon;
	}

	console.write(trainer->name + " has switched out to " + chosenPokemon->nickname + ".");
	console.write();
}

TrainerBattleScene::InternalAtk::InternalAtk(Pokemon* attacker, Attack* move)
	: attacker(attacker), move(move) {}

void TrainerBattleScene::pokemonWantsToUseMove(Pokemon* attacker, Attack* move) {
	atk.push_back(InternalAtk(attacker, move));
}

void TrainerBattleScene::pokemonUsesMove(Pokemon* attacker, Pokemon* target, Attack* move) {
	if (attacker->traits.obedience < random(0.0, 1.0)) {
		if (random(1, 2) == 4) {
			console.write(attacker->nickname + " refused to do anything.");
			console.write();
			return;
		}
		else {
			console.write(attacker->nickname + " decided to act on his own.");

			std::vector<Attack*> availableMoves = attacker->getAvailableMoves();
			move = availableMoves[random(0, availableMoves.size() - 1)];
		}
	}

	double chanceToHit = (double)move->accuracy / 100 *
		(attacker->currentStats.accuracy / target->currentStats.evasiveness);

	if (chanceToHit < random(0.0, 1.0)) {
		console.write(attacker->nickname + " missed!");
	}
	else {
		console.write(attacker->nickname + " used " + move->name + ".");
		--move->pp;

		int damage = move->getDamage(attacker, target);
		if (damage > 0) {
			target->receiveDamage(damage, move, attacker);
		}
	}

	console.write();
}

void TrainerBattleScene::checkIfPokemonFainted(Pokemon *& pokemon) {
	if (pokemon->getAvailableMoves().empty())
		pokemon->currentStats.hp = 0;

	if (pokemon->isFainted()) {
		console.write(pokemon->nickname + " has fainted.");
		console.write();

		pokemon = nullptr;

		animationIndex = 5;
		while (animationIndex);
	}
}

void TrainerBattleScene::restorePokemonToDefault(Trainer* trainer) {
	for (auto pokemon : trainer->pokemonInTeam) {
		int hp = pokemon->currentStats.hp;
		pokemon->currentStats = pokemon->maxStats;
		pokemon->currentStats.hp = hp;
		pokemon->effectsInBattle.clear();
		pokemon->timesBackOnFeet = 0;
	}
}