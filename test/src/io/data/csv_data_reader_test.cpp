#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>

#include "io/data/CsvDataReader.h"
#include "test-utils/ResultIs.h"

using ::testing::Return;

namespace {
    class MockMemory : public Memory {
    public:
        MOCK_CONST_METHOD1(value, std::optional<Command::Value>(Command::Reference ref));
        MOCK_METHOD2(setValue, void (Command::Reference ref, Command::Value value));
    };

    class MockParametersPathProvider : public ParametersPathProvider {
    public:
        MOCK_CONST_METHOD2(param, boost::filesystem::path(const std::string &name, bool requiredExists));
    };

    class CsvDataReaderTest : public ::testing::Test {
    protected:
        void SetUp() override {
            Test::SetUp();
            memory = std::make_unique<MockMemory>();
            provider = std::make_unique<MockParametersPathProvider>();

            meta = ParameterMetadata();
            meta.name = "a";
            meta.address = 1;
            meta.rows = 2;
            meta.columns = 3;
        }

        void TearDown() override {
            Test::TearDown();
            memory.reset();
            provider.reset();
        }

        std::unique_ptr<MockMemory> memory;
        std::unique_ptr<MockParametersPathProvider> provider;
        boost::filesystem::path testInputFilePath = "io/data/input.csv";
        ParameterMetadata meta;
    };
}

TEST_F(CsvDataReaderTest, read_input) {
    CsvDataReader reader(*memory, *provider);

    EXPECT_CALL(*provider, param(meta.name, true))
            .WillOnce(Return(testInputFilePath));

    for (int i = 1; i <= meta.size(); ++i) {
        EXPECT_CALL(*memory, setValue(i, ResultIs((double) i)));
    }

    EXPECT_NO_THROW(reader.readInput(std::vector{meta}));
}

TEST_F(CsvDataReaderTest, write_output) {
    CsvDataReader reader(*memory, *provider);

    EXPECT_CALL(*provider, param(meta.name, false))
            .WillOnce(Return(boost::filesystem::path("io/data/output.csv")));

    for (int i = 1; i <= meta.size(); ++i) {
        EXPECT_CALL(*memory, value(i)).WillOnce(Return(Command::Value((double) i)));
    }

    EXPECT_NO_THROW(reader.writeOutput(std::vector{meta}));

    std::ifstream outputFile("io/data/output.csv");
    std::string actualOutput{std::istreambuf_iterator<char>(outputFile), std::istreambuf_iterator<char>()};

    std::string expectedOutput = "1;2;3;\n"
                                 "4;5;6;\n"
                                 "";
    EXPECT_EQ(expectedOutput, actualOutput);
}
