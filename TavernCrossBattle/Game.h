#pragma once
#include <string>
#include "protocpp/Board.pb.h"

namespace battle {
	class Game {
	public:
		void Initialize(Player&& player1, Player&& player2);
		void LoopStep();
		void Attack(Minion *minion1, Minion *minion2);
		std::string GetBoardState();
	private:
		Board board_;
	};
}