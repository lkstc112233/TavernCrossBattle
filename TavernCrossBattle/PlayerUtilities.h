#pragma once

#include "protocpp/Player.pb.h"
#include "protocpp/Minion.pb.h"

namespace battle {
	namespace utilities {
		Minion* DetermineAttacker(Player* player);
		// Determines which minion attacks next. 
		// Called before each round.
		void DetermineNextAttacker(Player* player);
		Minion* DetermineDefender(Player* player);
		// Counts Alive Minions.
		int CountMinions(const Player& player);
		// Removes all dead minions from the minions list
		// except the ones holding the attack token.
		void RemoveDeadMinions(Player* player);
	}
}