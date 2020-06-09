#include "pch.h"

#include <google/protobuf/text_format.h>

#include "../TavernCrossBattle/Game.h"

#include "protocol-buffer-matchers.h"

using nucleus::EqualsProto;

class GameTest : public ::testing::Test {
protected:
	battle::Game game_;
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
			attacks_next: true
		}
	)", &player1);

	google::protobuf::TextFormat::ParseFromString(R"(
		minions {
			id: 2
			power: 2
			life_total: 3
			life_current: 3
			attacks_next: true
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
	}
	)"));
}

