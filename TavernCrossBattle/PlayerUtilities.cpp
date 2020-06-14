#include "PlayerUtilities.h"

#include <algorithm>

#include "MinionUtilities.h"
#include "RandomUtilities.h"
#include "protocpp/Player.pb.h"
#include "protocpp/Minion.pb.h"

namespace battle {
	namespace utilities {
		namespace {
			// Removes attack token from last attacked minion.
			void RemoveLastAttacker(Player* player) {
				Minion* lastAttacker = DetermineAttacker(player);
				if (lastAttacker) {
					lastAttacker->set_can_attack(false);
				}
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

		int CountMinions(const Player& player) {
			auto minions = player.minions();
			return std::count_if(std::begin(minions), std::end(minions), &IsAlive);
		}

		void RemoveDeadMinions(Player* player) {
			auto* minions = player->mutable_minions();
			minions->erase(std::remove_if(std::begin(*minions), std::end(*minions),
				[](const auto& minion) -> bool {
					return !IsAlive(minion) && !minion.can_attack();
				}), std::end(*minions));
		}

		bool StatusUpdate(Player* player) {
			if (!CountMinions(*player)) {
				return false;
			}
			RemoveDeadMinions(player);
			return true;
		}
	}
}