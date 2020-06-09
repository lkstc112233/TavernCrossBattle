#include "pch.h"

#include "../TavernCrossBattle/protocpp/Minion.pb.h"
#include "protocol-buffer-matchers.h"
using nucleus::EqualsProto;
TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(GMockIntegrationTest, TestName) {
    EXPECT_THAT(true, true);
}

TEST(GMockIntegrationTest, ProtoTest) {
    battle::Minion minion;

    EXPECT_THAT(minion, EqualsProto(minion));
    EXPECT_THAT(minion, EqualsProto(R"()"));
}