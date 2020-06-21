#include "Game.h"

#include <gmock/gmock.h>
#include <google/protobuf/text_format.h>
#include <gtest/gtest.h>

#include "protocol-buffer-matchers.h"

namespace battle {
	using nucleus::EqualsProto;
	namespace utilities {
		namespace internal_testing {
			void SetGeneratorSeed(int seed);
		}
	}

	class GameTest : public ::testing::Test {
	protected:
		void SetUp() override {
			battle::utilities::internal_testing::SetGeneratorSeed(0);
		}

		Game game_;
	};

	TEST_F(GameTest, InitializesCorrectlyAssigningId) {
		Player player1;
		Player player2;

		google::protobuf::TextFormat::ParseFromString(R"(
		minions {
			power: 3
			life_total: 3
			life_current: 3
		}
	)", &player1);

		google::protobuf::TextFormat::ParseFromString(R"(
		minions {
			power: 2
			life_total: 3
			life_current: 3
		}
	)", &player2);

		game_.Initialize(player1, player2);
		EXPECT_THAT(game_.GetBoardState(), EqualsProto(R"(
	game_status: ONGOING
	player1 {
		minions {
			id: 1
			power: 3
			life_total: 3
			life_current: 3
			can_attack: false
		}
	}
	player2 {
		minions {
			id: 2
			power: 2
			life_total: 3
			life_current: 3
			can_attack: false
		}
	}
	player1_attacks_next: false
	)"));
	}

	TEST_F(GameTest, LoopStepCorrectlyLoops) {
		Player player1;
		Player player2;

		google::protobuf::TextFormat::ParseFromString(R"(
		minions {
			power: 3
			life_total: 3
			life_current: 3
		}
	)", &player1);

		google::protobuf::TextFormat::ParseFromString(R"(
		minions {
			power: 2
			life_total: 3
			life_current: 3
		}
	)", &player2);

		game_.Initialize(player1, player2);
		game_.LoopStep();
		EXPECT_THAT(game_.GetBoardState(), EqualsProto(R"(
	game_status: ONGOING
	player1 {
		minions {
			id: 1
			power: 3
			life_total: 3
			life_current: 1
			can_attack: false
		}
	}
	player2 {
		minions {
			id: 2
			power: 2
			life_total: 3
			life_current: 0
			can_attack: true
		}
	}
	player1_attacks_next: false
	)"));
	}

}