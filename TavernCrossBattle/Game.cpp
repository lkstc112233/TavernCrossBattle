#include "Game.h"

#include <random>

#include "RandomUtilities.h"

namespace battle {
	using ::battle::utilities::RandomInt;
	using ::battle::utilities::RandomBool;

	namespace {
		Minion* DetermineAttacker(Player* player) {
			for (int i = 0; i < player->minions_size(); ++i) {
				if (player->minions(i).attacks_next()) {
					return player->mutable_minions(i);
				}
			}
			return nullptr;
		}
		void DetermineNextAttacker(Player* player) {
			// TODO: Implement this..
			if (player->minions_size()) {
				player->mutable_minions(0)->set_attacks_next(true);
				for (int i = 1; i < player->minions_size(); ++i) {
					player->mutable_minions(i)->set_attacks_next(false);
				}
			}
		}
		Minion* DetermineDefender(Player* player) {
			int minions = player->minions_size();
			return player->mutable_minions(RandomInt(0, minions - 1));
		}
	}

	void Game::Initialize(const Player& player1, const Player& player2) {
		board_.clear_player1();
		*board_.mutable_player1() = player1;
		DetermineNextAttacker(board_.mutable_player1());
		board_.clear_player2();
		*board_.mutable_player2() = player2;
		DetermineNextAttacker(board_.mutable_player2());
		if (RandomBool()) {
			board_.mutable_player1()->set_attacks_next(true);
			board_.mutable_player2()->set_attacks_next(false);
		} else {
			board_.mutable_player1()->set_attacks_next(false);
			board_.mutable_player2()->set_attacks_next(true);
		}
	}

	void Game::LoopStep() {
		Player* attackingPlayer = board_.player1().attacks_next() ? board_.mutable_player1() : board_.mutable_player2();
		Player* defendingPlayer = board_.player1().attacks_next() ? board_.mutable_player2() : board_.mutable_player1();
		Minion* attacker = DetermineAttacker(attackingPlayer);
		Minion* defender = DetermineDefender(defendingPlayer);
		Attack(attacker, defender);
	}
	void Game::Attack(Minion* attacker, Minion* defender) {
		attacker->set_life_current(attacker->life_current() - defender->power());
		defender->set_life_current(defender->life_current() - attacker->power());
	}
	const Board& Game::GetBoardState() {
		return board_;
	}
}
