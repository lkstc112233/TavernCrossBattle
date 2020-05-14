#pragma once
#include <string>
#include "protocpp/Board.pb.h"

namespace battle {
	class Game {
	public:
		void initialize(Player&& player1, Player&& player2);
		void loop();
		std::string getBoardState();
	private:
		Board board;
	};
}