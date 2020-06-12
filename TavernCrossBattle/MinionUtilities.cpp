#include "MinionUtilities.h"

#include "protocpp/Minion.pb.h"

namespace battle {
	namespace utilities {
		bool CanAttack(const Minion& minion) {
			return minion.power() > 0;
		}
	}
}