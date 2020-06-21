#include "Game.h"

#include <random>

#include "PlayerUtilities.h"
#include "RandomUtilities.h"

namespace battle {
	using ::battle::utilities::RandomBool;
	using ::battle::utilities::DetermineAttacker;
	using ::battle::utilities::DetermineNextAttacker;
	using ::battle::utilities::DetermineDefender;

	void Game::InitializePlayer(const Player& from, Player* to) {
		*to = from;
		for (auto& mutable_minion : *to->mutable_minions()) {
			mutable_minion.set_id(id_generator_.GenerateNextId());
		}
	}

	void Game::Initialize(const Player& player1, const Player& player2) {
		board_.set_game_status(Board::ONGOING);
		board_.clear_player1();
		InitializePlayer(player1, board_.mutable_player1());
		board_.clear_player2();
		InitializePlayer(player2, board_.mutable_player2());
		board_.set_player1_attacks_next(RandomBool());
	}

	void Game::LoopStep() {
		if (IsGameOver()) {
			return;
		}
		Player* attackingPlayer = board_.player1_attacks_next() ? board_.mutable_player1() : board_.mutable_player2();
		Player* defendingPlayer = board_.player1_attacks_next() ? board_.mutable_player2() : board_.mutable_player1();
		DetermineNextAttacker(attackingPlayer);
		Minion* attacker = DetermineAttacker(attackingPlayer);
		if (!attacker) {
			return;
		}
		Minion* defender = DetermineDefender(defendingPlayer);
		Attack(attacker, defender);
	}

	bool Game::IsGameOver() {
		switch (board_.game_status()) {
		case Board::PLAYER1_WINS:
		case Board::PLAYER2_WINS:
		case Board::DRAW:
			return true;
		default:
			return false;
		}
	}

	void Game::Attack(Minion* attacker, Minion* defender) {
		attacker->set_life_current(attacker->life_current() - defender->power());
		defender->set_life_current(defender->life_current() - attacker->power());
	}

	void Game::SwapAttackingPlayer() {
		board_.set_player1_attacks_next(!board_.player1_attacks_next());
	}

	void Game::StatusUpdate() {
		bool player1_alive = ::battle::utilities::StatusUpdate(board_.mutable_player1());
		bool player2_alive = ::battle::utilities::StatusUpdate(board_.mutable_player2());
		if (!player1_alive && !player2_alive) {
			board_.set_game_status(Board::DRAW);
		} else if (!player1_alive) {
			board_.set_game_status(Board::PLAYER2_WINS);
		} else if (!player2_alive) {
			board_.set_game_status(Board::PLAYER1_WINS);
		}
	}

	const Board& Game::GetBoardState() {
		return board_;
	}
}
