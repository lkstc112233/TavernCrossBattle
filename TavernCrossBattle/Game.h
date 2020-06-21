#pragma once
#include <string>
#include "protocpp/Board.pb.h"
#include "IdGenerator.h"

namespace battle {
	class Game {
	public:
		void Initialize(const Player& player1, const Player& player2);
		void LoopStep();
		const Board& GetBoardState();
		// Checks if the game status is over. 
		bool IsGameOver() const;

	private:

		// The attacking player attacks the defending player with a minion.
		void PerformAttack();

		// minion1 attacks minion2.
		void Attack(Minion* attacker, Minion* defender);

		// Checks status, winning, minion dying, etc.
		void StatusUpdate();

		// Swaps attacking player.
		void SwapAttackingPlayer();

		void InitializePlayer(const Player& from, Player* to);

		Board board_;
		utilities::IdGenerator id_generator_;
	};
}