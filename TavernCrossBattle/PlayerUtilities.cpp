#include "PlayerUtilities.h"

#include "RandomUtilities.h"
#include "protocpp/Player.pb.h"
#include "protocpp/Minion.pb.h"

namespace battle {
	namespace utilities {
		Minion* DetermineAttacker(Player* player) {
			for (int i = 0; i < player->minions_size(); ++i) {
				if (player->minions(i).attacks_next()) {
					return player->mutable_minions(i);
				}
			}
			return nullptr;
		}
		void DetermineNextAttacker(Player* player) {
			// TODO: Implement this..
			if (player->minions_size()) {
				player->mutable_minions(0)->set_attacks_next(true);
				for (int i = 1; i < player->minions_size(); ++i) {
					player->mutable_minions(i)->set_attacks_next(false);
				}
			}
		}
		Minion* DetermineDefender(Player* player) {
			int minions = player->minions_size();
			return player->mutable_minions(RandomInt(0, minions - 1));
		}
	}
}