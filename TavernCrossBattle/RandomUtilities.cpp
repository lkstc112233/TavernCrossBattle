#include "RandomUtilities.h"

#include <random>

namespace battle {
	namespace utilities {
		namespace {
			std::random_device rd;
			std::default_random_engine random_generator(rd());
			std::uniform_int_distribution<> bool_distribution(0, 1);
		}
		int RandomInt(int min, int max) {
			std::uniform_int_distribution<int> distribution(min, max);
			return distribution(random_generator);
		}
		bool RandomBool() {
			return bool_distribution(random_generator);
		}
		namespace internal_testing {
			void SetGeneratorSeed(int seed) {
				random_generator.seed(seed);
			}
		}
	}
}