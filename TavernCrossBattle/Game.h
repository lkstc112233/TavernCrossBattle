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

		// minion1 attacks minion2.
		void Attack(Minion *minion1, Minion *minion2);
	};
}