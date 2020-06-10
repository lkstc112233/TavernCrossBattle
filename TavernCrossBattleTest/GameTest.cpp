#include "Game.h"

#include "gtest/gtest.h"
#include <google/protobuf/text_format.h>

#include "protocol-buffer-matchers.h"

using nucleus::EqualsProto;

namespace battle {
	namespace utilities {
		namespace internal_testing {
			void SetGeneratorSeed(int seed);
		}
	}
}

class GameTest : public ::testing::Test {
protected:
	battle::Game game_;

	void SetUp() override {
		battle::utilities::internal_testing::SetGeneratorSeed(0);
	}
};

TEST_F(GameTest, InitializesCorrectly) {
	battle::Player player1;
	battle::Player player2;

	google::protobuf::TextFormat::ParseFromString(R"(
		minions {
			id: 1
			power: 3
			life_total: 3
			life_current: 3
		}
	)", &player1);

	google::protobuf::TextFormat::ParseFromString(R"(
		minions {
			id: 2
			power: 2
			life_total: 3
			life_current: 3
		}
	)", &player2);

	game_.Initialize(player1, player2);
	EXPECT_THAT(game_.GetBoardState(), EqualsProto(R"(
	player1 {
		minions {
			id: 1
			power: 3
			life_total: 3
			life_current: 3
			attacks_next: true
		}
	}
	player2 {
		minions {
			id: 2
			power: 2
			life_total: 3
			life_current: 3
			attacks_next: true
		}
		attacks_next: true
	}
	)"));
}

TEST_F(GameTest, LoopStepCorrectlyLoops) {
	battle::Player player1;
	battle::Player player2;

	google::protobuf::TextFormat::ParseFromString(R"(
		minions {
			id: 1
			power: 3
			life_total: 3
			life_current: 3
		}
	)", &player1);

	google::protobuf::TextFormat::ParseFromString(R"(
		minions {
			id: 2
			power: 2
			life_total: 3
			life_current: 3
		}
	)", &player2);

	game_.Initialize(player1, player2);
	game_.LoopStep();
	EXPECT_THAT(game_.GetBoardState(), EqualsProto(R"(
	player1 {
		minions {
			id: 1
			power: 3
			life_total: 3
			life_current: 1
			attacks_next: true
		}
	}
	player2 {
		minions {
			id: 2
			power: 2
			life_total: 3
			life_current: 0
			attacks_next: true
		}
		attacks_next: true
	}
	)"));
}
