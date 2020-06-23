#include "CollectionUtilities.h"

#include <gmock/gmock.h>
#include <google/protobuf/text_format.h>
#include <gtest/gtest.h>
#include <vector>

namespace battle {
	namespace utilities {
		TEST(FindIfExists, FindsElement) {
			std::vector<int> test_data{2, 5, 7};
			ASSERT_FALSE(find_if_exists(std::begin(test_data), std::end(test_data), [](const int& data) { return data == 1; }));
			ASSERT_TRUE(find_if_exists(std::begin(test_data), std::end(test_data), [](const int& data) { return data == 2; }));
			ASSERT_FALSE(find_if_exists(std::begin(test_data), std::end(test_data), [](const int& data) { return data == 3; }));
			ASSERT_FALSE(find_if_exists(std::begin(test_data), std::end(test_data), [](const int& data) { return data == 4; }));
			ASSERT_TRUE(find_if_exists(std::begin(test_data), std::end(test_data), [](const int& data) { return data == 5; }));
			ASSERT_TRUE(find_if_exists(std::begin(test_data), std::end(test_data), [](const int& data) { return data == 7; }));
			test_data.clear();
			ASSERT_FALSE(find_if_exists(std::begin(test_data), std::end(test_data), [](const int& data) { return data == 1; }));
			ASSERT_FALSE(find_if_exists(std::begin(test_data), std::end(test_data), [](const int& data) { return data == 2; }));
			ASSERT_FALSE(find_if_exists(std::begin(test_data), std::end(test_data), [](const int& data) { return data == 3; }));
			ASSERT_FALSE(find_if_exists(std::begin(test_data), std::end(test_data), [](const int& data) { return data == 4; }));
			ASSERT_FALSE(find_if_exists(std::begin(test_data), std::end(test_data), [](const int& data) { return data == 5; }));
			ASSERT_FALSE(find_if_exists(std::begin(test_data), std::end(test_data), [](const int& data) { return data == 7; }));
		}
	}
}