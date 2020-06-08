#include "Game.h"

#include <random>

namespace battle {
	namespace {
		std::default_random_engine random_generator;
		Minion* DetermineAttacker(Player* player) {
			for (int i = 0; i < player->minions_size(); ++i) {
				if (player->minions(i).attacks_next()) {
					return player->mutable_minions(i);
				}
			}
			return nullptr;
		}
		Minion* DetermineDefender(Player* player) {
			int minions = player->minions_size();
			std::uniform_int_distribution<int> distribution(0, minions - 1);
			return player->mutable_minions(distribution(random_generator));
		}
	}

	void Game::Initialize(const Player& player1, const Player& player2) {
		board_.clear_player1();
		*board_.mutable_player1() = player1;
		board_.clear_player2();
		*board_.mutable_player2() = player2;
	}

	void Game::LoopStep() {

	}
	void Game::Attack(Minion* attacker, Minion* defender) {
		attacker->set_life_current(attacker->life_current() - defender->power());
		defender->set_life_current(defender->life_current() - attacker->power());
	}
	const Board& Game::GetBoardState() {
		return board_;
	}
}
