#include <gtest/gtest.h>
#include "utils.h"

/**
 * Тест сумматора.
 */
TEST(utils_test, sum) {
    EXPECT_EQ(3, sum(1, 2));
}
