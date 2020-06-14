#pragma once
#include <string>
#include "protocpp/Board.pb.h"

namespace battle {
	class Game {
	public:
		void Initialize(const Player& player1, const Player& player2);
		void LoopStep();
		const Board& GetBoardState();
	private:
		Board board_;

		// Checks if the game status is over. 
		bool IsGameOver();

		// minion1 attacks minion2.
		void Attack(Minion* attacker, Minion* defender);
	};
}