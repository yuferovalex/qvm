#include <gtest/gtest.h>

#include "io/command/ProgramFileReaderV1.h"

#include "test-utils/PseudoAssemblerLanguage.h"

/**
 * Тест проверяет работоспособность класса {@link CommandFileReaderV1} на примере
 * программы, составленной вручную.
 */
TEST(command_file_reader_v1, test) {
    ProgramFileReaderV1 reader("io/command/product2x2.qvm");

    EXPECT_EQ(20, reader.memorySize());
    EXPECT_EQ("Calculates 2-nd order matrix product.", reader.programDescription());

    auto inputParam = reader.inputParamsMeta();
    EXPECT_EQ(2, inputParam.size());

    EXPECT_EQ("a", inputParam[0].name);
    EXPECT_EQ("Left operand of product.", inputParam[0].description);
    EXPECT_EQ(1, inputParam[0].address);
    EXPECT_EQ(2, inputParam[0].rows);
    EXPECT_EQ(2, inputParam[0].columns);
    EXPECT_EQ(4, inputParam[0].size());

    EXPECT_EQ("b", inputParam[1].name);
    EXPECT_EQ("Right operand of product.", inputParam[1].description);
    EXPECT_EQ(5, inputParam[1].address);
    EXPECT_EQ(2, inputParam[1].rows);
    EXPECT_EQ(2, inputParam[1].columns);
    EXPECT_EQ(4, inputParam[1].size());

    auto outputParam = reader.outputParamsMeta();
    EXPECT_EQ(1, outputParam.size());

    EXPECT_EQ("c", outputParam[0].name);
    EXPECT_EQ("Product of a and b.", outputParam[0].description);
    EXPECT_EQ(9, outputParam[0].address);
    EXPECT_EQ(2, outputParam[0].columns);
    EXPECT_EQ(2, outputParam[0].rows);
    EXPECT_EQ(4, outputParam[0].size());

    auto expectedProgram = QVM_ASM_BEGIN(std::vector<Command>())
        CMD_MUL(REF(1), REF(5), 13);
        CMD_MUL(REF(2), REF(7), 14);
        CMD_ADD(REF(13), REF(14), 9);

        CMD_MUL(REF(1), REF(6), 15);
        CMD_MUL(REF(2), REF(8), 16);
        CMD_ADD(REF(15), REF(16), 10);

        CMD_MUL(REF(3), REF(5), 17);
        CMD_MUL(REF(4), REF(7), 18);
        CMD_ADD(REF(17), REF(18), 11);

        CMD_MUL(REF(3), REF(6), 19);
        CMD_MUL(REF(4), REF(8), 20);
        CMD_ADD(REF(19), REF(20), 12);
            QVM_ASM_END;

    auto expectedIt = expectedProgram.begin();
    auto actualIt = reader.commandStreamBegin();

    while (expectedIt != expectedProgram.end() && actualIt != reader.commandStreamEnd()) {
        auto expected = *expectedIt++;
        auto actual = *actualIt++;

        EXPECT_EQ(expected.operation, actual.operation);
        EXPECT_EQ(expected.lhsOperand.ref, actual.lhsOperand.ref);
        EXPECT_EQ(expected.rhsOperand.ref, actual.rhsOperand.ref);
        EXPECT_EQ(expected.resultRef, actual.resultRef);
    }

    EXPECT_EQ(expectedProgram.end(), expectedIt);
    EXPECT_EQ(reader.commandStreamEnd(), actualIt);
}
