#include "PlayerUtilities.h"

#include "RandomUtilities.h"
#include "protocpp/Player.pb.h"
#include "protocpp/Minion.pb.h"

namespace battle {
	namespace utilities {
		namespace {
			bool CanAttack(const Minion& minion) {
				return minion.power() > 0;
			}
		}
		Minion* DetermineAttacker(Player* player) {
			for (int i = 0; i < player->minions_size(); ++i) {
				if (player->minions(i).can_attack()) {
					return player->mutable_minions(i);
				}
			}
			return nullptr;
		}

		void RemoveLastAttacker(Player* player) {
			Minion* lastAttacker = DetermineAttacker(player);
			if (lastAttacker) {
				lastAttacker->set_can_attack(false);
			}
		}

		void DetermineNextAttacker(Player* player) {
			RemoveLastAttacker(player);
			for (Minion &minion: *player->mutable_minions()) {
				if (minion.can_attack()) {
					if (CanAttack(minion)) {
						return;
					}
					minion.set_can_attack(false);
				}
			}
			bool met_can_attack = false;
			for (Minion& minion : *player->mutable_minions()) {
				if (met_can_attack || CanAttack(minion)) {
					met_can_attack = true;
					minion.set_can_attack(true);
				}
			}
		}

		Minion* DetermineDefender(Player* player) {
			int minions = player->minions_size();
			return player->mutable_minions(RandomInt(0, minions - 1));
		}
	}
}