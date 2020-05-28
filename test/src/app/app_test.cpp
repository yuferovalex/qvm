#include <gtest/gtest.h>

#include "app/SharedMemoryApplication.h"

TEST(app_test, product) {
    auto args = std::vector {
        "qvm", "-p", "resources/product2x2.qvm",
                "--param:a", "resources/input.csv",
                "--param:b", "resources/input.csv",
                "--param:c", "product_output.csv"
    };

    SharedMemoryApplication app(args.size(), args.data());

    EXPECT_NO_THROW({
        EXPECT_EQ(EXIT_SUCCESS, app.run());
    });
}

TEST(app_test, sqare_eq) {
    auto args = std::vector {
            "qvm", "-p", "resources/sqare_eq/program.qvm",
            "--param:a", "resources/sqare_eq/a.csv",
            "--param:b", "resources/sqare_eq/b.csv",
            "--param:c", "resources/sqare_eq/c.csv",
            "--param:x1", "sqare_eq_output_x1.csv",
            "--param:x2", "sqare_eq_output_x2.csv",
    };

    SharedMemoryApplication app(args.size(), args.data());

    EXPECT_NO_THROW({
        EXPECT_EQ(EXIT_SUCCESS, app.run());
    });
}

TEST(app_test, zhordan) {
    auto args = std::vector {
            "qvm", "-p", "resources/zhordan/program.qvm",
            "--param:A", "resources/zhordan/A.csv",
            "--param:X", "zhordan_output.csv",
    };

    SharedMemoryApplication app(args.size(), args.data());

    EXPECT_NO_THROW({
        EXPECT_EQ(EXIT_SUCCESS, app.run());
    });
}