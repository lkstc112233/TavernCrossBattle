#pragma once
#include <string>
#include "protocpp/Board.pb.h"

namespace battle {
	class Game {
	public:
		void Initialize(const Player& player1, const Player& player2);
		void LoopStep();
		void Attack(Minion *minion1, Minion *minion2);
		const Board& GetBoardState();
	private:
		Board board_;
	};
}