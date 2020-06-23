#include "MinionUtilities.h"

#include "CollectionUtilities.h"
#include "protocpp/Minion.pb.h"

namespace battle {
	namespace utilities {
		bool CanAttack(const Minion& minion) {
			return minion.power() > 0;
		}

		bool IsAlive(const Minion& minion) {
			return minion.life_current() > 0;
		}

		bool HasKeywordAbility(const Minion& minion, Ability::KeywordAbilities keyword) {
			return find_if_exists(
				std::begin(minion.abilities()),
				std::end(minion.abilities()),
				[keyword](const Ability& ability) -> bool {
					return ability.keyword() == keyword;
				}
			);
		}
	}
}