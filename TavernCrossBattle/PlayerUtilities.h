#pragma once

#include "protocpp/Player.pb.h"
#include "protocpp/Minion.pb.h"

namespace battle {
	namespace utilities {
		Minion* DetermineAttacker(Player* player);
		void DetermineNextAttacker(Player* player);
		Minion* DetermineDefender(Player* player);
	}
}