#include "IdGenerator.h"

namespace battle {
	namespace utilities {
		int IdGenerator::GenerateNextId() {
			return ++id;
		}
	}
}