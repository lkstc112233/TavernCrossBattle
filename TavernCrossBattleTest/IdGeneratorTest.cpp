#include "IdGenerator.h"

#include <gmock/gmock.h>
#include <google/protobuf/text_format.h>
#include <gtest/gtest.h>

#include "protocol-buffer-matchers.h"
#include "protocpp/Player.pb.h"

namespace battle {
	namespace utilities {
		class IdGeneratorTest : public ::testing::Test {
		protected:
			IdGenerator  id_generator_;
		};

		TEST_F(IdGeneratorTest, TestGenerateId) {
			ASSERT_EQ(id_generator_.GenerateNextId(), 1);
			ASSERT_EQ(id_generator_.GenerateNextId(), 2);
			ASSERT_EQ(id_generator_.GenerateNextId(), 3);
			ASSERT_EQ(id_generator_.GenerateNextId(), 4);
			ASSERT_EQ(id_generator_.GenerateNextId(), 5);
			ASSERT_EQ(id_generator_.GenerateNextId(), 6);
		}
	}
}