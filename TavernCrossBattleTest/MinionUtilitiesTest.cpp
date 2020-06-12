#include "MinionUtilities.h"

#include <gmock/gmock.h>
#include <google/protobuf/text_format.h>
#include <gtest/gtest.h>

#include "protocol-buffer-matchers.h"
#include "protocpp/Minion.pb.h"

namespace battle {
	namespace utilities {
		TEST(CanAttack, NormalMinionCanAttack) {
			Minion minion;

			google::protobuf::TextFormat::ParseFromString(R"(
					id: 1
					power: 3
					life_total: 3
					life_current: 3
				)", &minion);

			EXPECT_TRUE(CanAttack(minion));
		}
		TEST(CanAttack, MinionWithoutPowerCanAttack) {
			Minion minion;

			google::protobuf::TextFormat::ParseFromString(R"(
					id: 1
					power: 0
					life_total: 3
					life_current: 3
				)", &minion);

			EXPECT_FALSE(CanAttack(minion));
		}
	}
}