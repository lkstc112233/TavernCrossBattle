#pragma once

#include "protocpp/Ability.pb.h"
#include "protocpp/Minion.pb.h"

namespace battle {
	namespace utilities {
		// Determines if a minion can attack.
		bool CanAttack(const Minion& minion);
		// Determines if a minion is alive.
		bool IsAlive(const Minion& minion);
		// Determines if a minion has certain keyword ability.
		bool HasKeywordAbility(const Minion& minion, Ability::KeywordAbilities keyword);
	}
}