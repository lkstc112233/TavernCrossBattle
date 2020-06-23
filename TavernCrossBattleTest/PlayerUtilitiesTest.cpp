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
	TEST(DetermineDefender, ReturnsDefender) {
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

		Minion* minion = utilities::DetermineDefender(&player);

		ASSERT_NE(minion, nullptr);
		EXPECT_THAT(*minion, AnyOf(EqualsProto(R"(
				id: 1
				power: 3
				life_total: 3
				life_current: 3
				can_attack: false
			)"), EqualsProto(R"(
				id: 2
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			)")));
	}

	TEST(DetermineDefender, ReturnsDefenderExcludingDeadMinions) {
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
				life_current: -3
				can_attack: true
			}
		)", &player);

		Minion* minion = utilities::DetermineDefender(&player);

		ASSERT_NE(minion, nullptr);
		EXPECT_THAT(*minion, EqualsProto(R"(
				id: 1
				power: 3
				life_total: 3
				life_current: 3
				can_attack: false
			)"));
	}

	TEST(DetermineDefender, ReturnsTauntMinionsBeforeOthers) {
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
				life_current: -3
				can_attack: true
			}
			minions {
				id: 3
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
				abilities {
					keyword: TAUNT
				}
			}
			minions {
				id: 4
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			}
			minions {
				id: 5
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			}
			minions {
				id: 6
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
				abilities {
					keyword: TAUNT
				}
			}
		)", &player);

		Minion* minion = utilities::DetermineDefender(&player);

		ASSERT_NE(minion, nullptr);
		EXPECT_THAT(*minion, AnyOf(EqualsProto(R"(
				id: 3
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
				abilities {
					keyword: TAUNT
				}
			)"), EqualsProto(R"(
				id: 6
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
				abilities {
					keyword: TAUNT
				}
			)")));
	}

	TEST(DetermineDefender, ExcludesDeadTauntMinions) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: -3
				can_attack: false
				abilities {
					keyword: TAUNT
				}
			}
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			}
			minions {
				id: 3
				power: 5
				life_total: 12
				life_current: -3
				can_attack: true
				abilities {
					keyword: TAUNT
				}
			}
			minions {
				id: 4
				power: 5
				life_total: 12
				life_current: -3
				can_attack: true
				abilities {
					keyword: TAUNT
				}
			}
			minions {
				id: 5
				power: 5
				life_total: 12
				life_current: -3
				can_attack: true
				abilities {
					keyword: TAUNT
				}
			}
			minions {
				id: 6
				power: 5
				life_total: 12
				life_current: -3
				can_attack: true
				abilities {
					keyword: TAUNT
				}
			}
		)", &player);

		Minion* minion = utilities::DetermineDefender(&player);

		ASSERT_NE(minion, nullptr);
		EXPECT_THAT(*minion, EqualsProto(R"(
				id: 2
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			)"));
	}

	TEST(CountTauntMinions, CountsTauntMinions) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 3
				can_attack: false
				abilities {
					keyword: TAUNT
				}
			}
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			}
		)", &player);

		ASSERT_EQ(utilities::CountTauntMinions(player), 1);
	}

	TEST(CountTauntMinions, CountsAllTauntMinions) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 3
				can_attack: false
				abilities {
					keyword: TAUNT
				}
			}
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
				abilities {
					keyword: TAUNT
				}
			}
		)", &player);

		ASSERT_EQ(utilities::CountTauntMinions(player), 2);
	}

	TEST(CountTauntMinions, CountsTauntMinionsNotCountingDeadMinions) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: -3
				can_attack: false
				abilities {
					keyword: TAUNT
				}
			}
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
				abilities {
					keyword: TAUNT
				}
			}
		)", &player);

		ASSERT_EQ(utilities::CountTauntMinions(player), 1);
	}

	TEST(CountMinions, ReturnsCountOfAliveMinions) {
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

		ASSERT_EQ(utilities::CountMinions(player), 2);
	}

	TEST(CountMinions, ReturnsZeroForEmptyMinions) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"()", &player);

		ASSERT_EQ(utilities::CountMinions(player), 0);
	}
	TEST(CountMinions, NotCountingDeadMinions) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 0
				can_attack: false
			}
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: -3
				can_attack: true
			}
			minions {
				id: 3
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			}
		)", &player);

		ASSERT_EQ(utilities::CountMinions(player), 1);
	}

	TEST(RemoveDeadMinions, DoesNotRemoveAliveMinions) {
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

		utilities::RemoveDeadMinions(&player);
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

	TEST(RemoveDeadMinions, DoesNotAffectEmptyMinions) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"()", &player);

		utilities::RemoveDeadMinions(&player);

		EXPECT_THAT(player, EqualsProto(R"()"));
	}
	TEST(RemoveDeadMinions, RemovesDeadMinionWithoutAttackingToken) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 0
				can_attack: false
			}
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: -3
				can_attack: true
			}
			minions {
				id: 3
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			}
		)", &player);

		utilities::RemoveDeadMinions(&player);

		EXPECT_THAT(player, EqualsProto(R"(
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: -3
				can_attack: true
			}
			minions {
				id: 3
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			})"));
	}

	TEST(RemoveDeadMinions, RemovesAllDeadMinionWithoutAttackingToken) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 0
				can_attack: false
			}
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: -3
				can_attack: false
			}
			minions {
				id: 3
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			}
		)", &player);

		utilities::RemoveDeadMinions(&player);

		EXPECT_THAT(player, EqualsProto(R"(
			minions {
				id: 3
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			})"));
	}

	TEST(StatusUpdate, DoesNotRemoveAliveMinions) {
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

		ASSERT_EQ(utilities::StatusUpdate(&player), true);
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

	TEST(StatusUpdate, ReturnsFalseForEmptyMinions) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"()", &player);

		ASSERT_EQ(utilities::StatusUpdate(&player), false);
		EXPECT_THAT(player, EqualsProto(R"()"));
	}

	TEST(StatusUpdate, ReturnsFalseForAllDeadMinions) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 0
				can_attack: false
			}
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: -3
				can_attack: true
			})", &player);

		ASSERT_EQ(utilities::StatusUpdate(&player), false);
		EXPECT_THAT(player, EqualsProto(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 0
				can_attack: false
			}
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: -3
				can_attack: true
			})"));
	}

	TEST(StatusUpdate, RemovesDeadMinionWithoutAttackingToken) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 0
				can_attack: false
			}
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: -3
				can_attack: true
			}
			minions {
				id: 3
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			}
		)", &player);

		ASSERT_EQ(utilities::StatusUpdate(&player), true);
		EXPECT_THAT(player, EqualsProto(R"(
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: -3
				can_attack: true
			}
			minions {
				id: 3
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			})"));
	}

	TEST(StatusUpdate, RemovesAllDeadMinionWithoutAttackingToken) {
		Player player;

		google::protobuf::TextFormat::ParseFromString(R"(
			minions {
				id: 1
				power: 3
				life_total: 3
				life_current: 0
				can_attack: false
			}
			minions {
				id: 2
				power: 5
				life_total: 12
				life_current: -3
				can_attack: false
			}
			minions {
				id: 3
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			}
		)", &player);

		ASSERT_EQ(utilities::StatusUpdate(&player), true);
		EXPECT_THAT(player, EqualsProto(R"(
			minions {
				id: 3
				power: 5
				life_total: 12
				life_current: 3
				can_attack: true
			})"));
	}
}