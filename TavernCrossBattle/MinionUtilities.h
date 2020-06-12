#pragma once

#include "protocpp/Minion.pb.h"

namespace battle {
	namespace utilities {
		// Determines if a minion can attack.
		bool CanAttack(const Minion& minion);
	}
}