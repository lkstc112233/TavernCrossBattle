#include "Game.h"

namespace battle {
	void Game::Initialize(const Player& player1, const Player& player2) {
		board_.clear_player1();
		*board_.mutable_player1() = player1;
		board_.clear_player2();
		*board_.mutable_player2() = player2;
	}

	void Game::LoopStep() {

	}
	void Game::Attack(Minion* minion1, Minion* minion2) {
		minion1->set_life_current(minion1->life_current() - minion2->power());
		minion2->set_life_current(minion2->life_current() - minion1->power());
	}
	const Board& Game::GetBoardState() {
		return board_;
	}
}
