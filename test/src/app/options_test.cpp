#include <gtest/gtest.h>
#include <thread>
#include <dispatcher/DispatcherConfigImpl.h>

#include "app/Options.h"

TEST(vm_options, help) {
    std::vector<const char *> args {
        "/qvm", "--help"
    };

    Options options(args.size(), &args[0]);

    EXPECT_TRUE(options.isShowHelp());
}

TEST(vm_options, help_short_notation) {
    std::vector<const char *> args {
            "/qvm", "-h"
    };

    Options options(args.size(), &args[0]);

    EXPECT_TRUE(options.isShowHelp());
}

TEST(vm_options, help_default_value_must_be_false) {
    std::vector<const char *> args {
            "/qvm"
    };

    Options options(args.size(), &args[0]);

    EXPECT_FALSE(options.isShowHelp());
}

TEST(vm_options, version) {
    std::vector<const char *> args {
            "/qvm", "--version"
    };

    Options options(args.size(), &args[0]);

    EXPECT_TRUE(options.isShowVersion());
}

TEST(vm_options, version_short_notation) {
    std::vector<const char *> args {
            "/qvm", "-v"
    };

    Options options(args.size(), &args[0]);

    EXPECT_TRUE(options.isShowVersion());
}

TEST(vm_options, version_default_value_must_be_false) {
    std::vector<const char *> args {
            "/qvm"
    };

    Options options(args.size(), &args[0]);

    EXPECT_FALSE(options.isShowVersion());
}

TEST(vm_options, parameters) {
    std::vector<const char *> args {
            "/qvm", "--info"
    };

    Options options(args.size(), &args[0]);

    EXPECT_TRUE(options.isShowProgramInfo());
}

TEST(vm_options, parameters_default_value_must_be_false) {
    std::vector<const char *> args {
            "/qvm"
    };

    Options options(args.size(), &args[0]);

    EXPECT_FALSE(options.isShowProgramInfo());
}

TEST(vm_options, program_path) {
    std::vector<const char *> args {
            "/qvm", "--program", "/path/to/prog.qvm"
    };

    Options options(args.size(), &args[0]);

    EXPECT_EQ("/path/to/prog.qvm", options.programPath().string());
}

TEST(vm_options, wrong_program_path) {
    std::vector<const char *> args {
            "/qvm", "--program", "/:<>?\\|.qvm"
    };

    EXPECT_NO_THROW(Options (args.size(), &args[0]));
}

TEST(vm_options, show_description) {
    std::vector<const char *> args {
            "/qvm"
    };

    EXPECT_NO_THROW(Options (args.size(), &args[0]).showOptionsDescription());
}

TEST(vm_options, threads_count) {
    std::vector<const char *> args {
            "/qvm", "--threads", "2"
    };

    Options options(args.size(), &args[0]);

    EXPECT_EQ(2, options.threadCount());
}

TEST(vm_options, threads_count_short_notation) {
    std::vector<const char *> args {
            "/qvm", "-t", "2"
    };

    Options options(args.size(), &args[0]);

    EXPECT_EQ(2, options.threadCount());
}

TEST(vm_options, threads_count_default_value) {
    std::vector<const char *> args {
            "/qvm"
    };

    Options options(args.size(), &args[0]);

    EXPECT_EQ(DispatcherConfigImpl::DEFAULT_THREAD_COUNT, options.threadCount());
}

TEST(vm_options, batch_count) {
    std::vector<const char *> args {
            "/qvm", "--batch", "2"
    };

    Options options(args.size(), &args[0]);

    EXPECT_EQ(2, options.batchSize());
}

TEST(vm_options, batch_short_notation) {
    std::vector<const char *> args {
            "/qvm", "-b", "2"
    };

    Options options(args.size(), &args[0]);

    EXPECT_EQ(2, options.batchSize());
}

TEST(vm_options, batch_default_value) {
    std::vector<const char *> args {
            "/qvm"
    };

    Options options(args.size(), &args[0]);

    EXPECT_EQ(DispatcherConfigImpl::DEFAULT_BATCH_SIZE, options.batchSize());
}