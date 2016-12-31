#include "stdafx.h"
#include "Game\Game.h"
#include "Game\Scene.h"
#include "Pokemons\Types.h"
#include <assert.h>

Scene* Scene::instance = nullptr;
std::atomic<int> Scene::userActionsDisabledCount = 0;

Scene::Scene() {
	userActionsDisabledCount = 0;
}

Scene* Scene::getInstance() {
	return instance;
}

void Scene::updateInstance(Scene* inst) {
	if (instance)
		delete instance;

	instance = inst;
}

void Scene::releaseInstance() {
	delete instance;
}

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

	// draw current battling pokemon

	if(pokemonA == NULL)
		pokemonA = trainerA->pokemonInTeam[0];
	pokemonB = trainerB->pokemonInTeam[0];

	Position<float> 
		posPokemonBack(unitSize.width * 170, unitSize.height * 210),
		posPokemonFront(unitSize.width * 540, unitSize.height * 90);
	Size<float> pokemonSpriteSize(96 * unitSize.width, 96 * unitSize.height);

	Graphics::PokemonInBattle pokemonBack(pokemonA, "back", pokemonSpriteSize, posPokemonBack);
	Graphics::PokemonInBattle pokemonFront(pokemonB, "front", pokemonSpriteSize, posPokemonFront);
	pokemonBack.draw(window);
	pokemonFront.draw(window);

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
			pokemonA->moves.size() > i ? pokemonA->moves[i].name : "-", 
			attackBtnSize, attackBtnPos, attackBtnColor
		);

		button.draw(window);

		if (pokemonA->moves.size() > i) {
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
		if (!trainerA->pokemonInTeam[i]->isFainted() && trainerA->pokemonInTeam[i] != pokemonA) {
			pokemonInTeam.checkIfClicked(window, [](const std::vector<void*>& args) {
				// on 1 there is the type of action (0 = change pokemon)
				pokemonBarFctArgs[1] = (void*)0;
				MainTrainer* trainer = (MainTrainer*)args[0];
				trainer->notify(args);

			}, pokemonBarFctArgs);
		}
	}
}

void TrainerBattleScene::logic() {
	atk.clear();

	trainerA->takeTurnInBattle();

	++Scene::userActionsDisabledCount;
	trainerB->takeTurnInBattle();

	if (atk.size() == 2)
		if (atk[0].attacker->currentStats.speed < atk[1].attacker->currentStats.speed)
			std::swap(atk[0], atk[1]);

	if(atk.size() >= 1 && !atk[0].attacker->isFainted())
		pokemonUsesMove(atk[0].attacker, atk[0].target, atk[0].move);

	if (atk.size() == 2 && !atk[1].attacker->isFainted())
		pokemonUsesMove(atk[1].attacker, atk[1].target, atk[1].move);

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
			pokemonA = chosenPokemon;
		}
		else {
			console.write(pokemonA->nickname + " refused to leave the battle.");
			return;
		}
	}
	else {
		pokemonB = chosenPokemon;
	}

	console.write(trainer->name + " has switched out to " + chosenPokemon->nickname + ".");
}

TrainerBattleScene::InternalAtk::InternalAtk(Pokemon* attacker, Pokemon* target, Attack* move)
	: attacker(attacker), target(target), move(move) {}

void TrainerBattleScene::pokemonWantsToUseMove(Pokemon* attacker, Pokemon* target, Attack* move) {
	atk.push_back(InternalAtk(attacker, target, move));
}

void TrainerBattleScene::pokemonUsesMove(Pokemon* attacker, Pokemon* target, Attack* move) {
	if (attacker->traits.obedience < random(0.0, 1.0)) {
		if (random(1, 2) == 4) {
			console.write(attacker->nickname + " refused to do anything.");
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