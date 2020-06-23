#include "PlayerUtilities.h"

#include <algorithm>
#include <functional>

#include "CollectionUtilities.h"
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

			template<typename Iterator, typename Pred, typename Counter>
			Iterator find_if_nth(Iterator first, Iterator last, Counter n, Pred closure) {
				typedef typename std::iterator_traits<Iterator>::reference Tref;
				return std::find_if(first, last, [&](const Tref x) {
					return closure(x) && !(n--);
					});
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
			int minions_count = CountTauntMinions(*player);
			if (minions_count) {
				auto minions = player->mutable_minions();
				int minions_index = RandomInt(0, minions_count - 1);
				return &*find_if_nth(std::begin(*minions), std::end(*minions),
					minions_index, 
					std::bind(std::logical_and<void>(), 
						std::bind(&IsAlive, std::placeholders::_1), 
						std::bind(&HasKeywordAbility, std::placeholders::_1, Ability::TAUNT)));
			}
			minions_count = CountMinions(*player);
			if (minions_count == 0) {
				return nullptr;
			}
			auto minions = player->mutable_minions();
			int minions_index = RandomInt(0, minions_count - 1);
			return &*find_if_nth(std::begin(*minions), std::end(*minions),
				minions_index, &IsAlive);
		}

		int CountTauntMinions(const Player& player) {
			auto minions = player.minions();
			return std::count_if(
				std::begin(minions),
				std::end(minions),
				[](const Minion& minion) {
					return IsAlive(minion) && HasKeywordAbility(minion, Ability::TAUNT);
				});
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