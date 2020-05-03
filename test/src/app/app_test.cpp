#include <gtest/gtest.h>

#include "app/SharedMemoryApplication.h"

TEST(app_test, test) {
    auto args = std::vector {
        "qvm", "-p", "resources/product2x2.qvm",
                "--param:a", "resources/input.csv",
                "--param:b", "resources/input.csv",
                "--param:c", "app_test_output.csv"
    };

    SharedMemoryApplication app(args.size(), args.data());

    EXPECT_NO_THROW({
        EXPECT_EQ(EXIT_SUCCESS, app.run());
    });
}
