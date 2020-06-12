#include "PlayerUtilities.h"

#include <gmock/gmock.h>
#include <google/protobuf/text_format.h>
#include <gtest/gtest.h>

#include "protocol-buffer-matchers.h"
#include "protocpp/Player.pb.h"

namespace battle {
	using nucleus::EqualsProto;
	TEST(DetermineAttacker, ReturnsNullForEmptyBoard) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"(
			minions {
			}
		)", &player);

		Minion* minion = utilities::DetermineAttacker(&player);

		ASSERT_EQ(minion, nullptr);
	}

	TEST(DetermineAttacker, ReturnsNullIfEveryMinionIsNotMarkedAsAttacksNext) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 3
				can_attack: false
			}
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: 3
				can_attack: false
			}
		)", &player);

		Minion* minion = utilities::DetermineAttacker(&player);

		ASSERT_EQ(minion, nullptr);
	}

	TEST(DetermineAttacker, ReturnsTheFirstAttackableMinion) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 3
				can_attack: false
			}
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			}
		)", &player);

		Minion* minion = utilities::DetermineAttacker(&player);

		ASSERT_NE(minion, nullptr);
		EXPECT_THAT(*minion, EqualsProto(R"(
				id: 2
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			)"));
	}

	TEST(DetermineNextAttacker, ModifiesPlayerToShowAttacker) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 3
				can_attack: false
			}
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			}
		)", &player);

		utilities::DetermineNextAttacker(&player);

		EXPECT_THAT(player, EqualsProto(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 3
				can_attack: true
			}
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			}
		)"));
		utilities::DetermineNextAttacker(&player);

		EXPECT_THAT(player, EqualsProto(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 3
				can_attack: false
			}
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			}
		)"));
	}

	TEST(DetermineNextAttacker, SkipsMinionsCannotAttack) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 3
				can_attack: false
			}
			minions {
				id: 2
				power: 0
				life_total: 3
				life_current: 3
				can_attack: false
			}
			minions {
				id: 3
				power: 5
				life_total: 12
				life_current: 3
				can_attack: false
			}
		)", &player);

		utilities::DetermineNextAttacker(&player);

		EXPECT_THAT(player, EqualsProto(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 3
				can_attack: true
			}
			minions {
				id: 2
				power: 0
				life_total: 3
				life_current: 3
				can_attack: true
			}
			minions {
				id: 3
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			}
		)"));

		utilities::DetermineNextAttacker(&player);

		EXPECT_THAT(player, EqualsProto(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 3
				can_attack: false
			}
			minions {
				id: 2
				power: 0
				life_total: 3
				life_current: 3
				can_attack: false
			}
			minions {
				id: 3
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			}
		)"));

		utilities::DetermineNextAttacker(&player);

		EXPECT_THAT(player, EqualsProto(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 3
				can_attack: true
			}
			minions {
				id: 2
				power: 0
				life_total: 3
				life_current: 3
				can_attack: true
			}
			minions {
				id: 3
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			}
		)"));
	}
}