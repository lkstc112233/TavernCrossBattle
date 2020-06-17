#pragma once

#include <atomic>

namespace battle {
	namespace utilities {
		class IdGenerator {
		public:
			int GenerateNextId();
		private:
			std::atomic_int id = 0;
		};
	}
}

