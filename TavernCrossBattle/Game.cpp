#include "Game.h"

namespace battle {
	void Game::initialize(Player&& player1, Player&& player2) {
		board.clear_player1();
		*board.mutable_player1() = player1;
		board.clear_player2();
		*board.mutable_player2() = player2;
	}

	void Game::loop() {

	}
	std::string Game::getBoardState()
	{
		return board.DebugString();
	}
}
