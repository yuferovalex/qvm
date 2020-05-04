#include <variant>
#include <cmath>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "interpreter/CommandHandlersImpl.h"
#include "memory/Memory.h"
#include "memory/StringVault.h"
#include "utils/CancellationToken.h"
#include "test-utils/ResultIs.h"
#include "test-utils/PseudoAssemblerLanguage.h"

using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::_;

namespace {

    class MockStringVault : public StringVault {
    public:
        MOCK_CONST_METHOD1(string, const std::string &(uint32_t id));
        MOCK_METHOD1(addString, uint32_t (const std::string &string));
    };

    class MockMemory : public Memory {
    public:
        MOCK_CONST_METHOD1(value, std::optional<Command::Value> (Command::Reference ref));
        MOCK_METHOD2(setValue, void (Command::Reference ref, Command::Value value));
    };

    typedef std::tuple<std::shared_ptr<CommandHandler>, Command::Value, Command::Value, Result> Params;

    class CommandHandlersTest :
            public ::testing::Test,
            public ::testing::WithParamInterface<Params> {
    protected:
        MockMemory memory;
    };
}

INSTANTIATE_TEST_CASE_P(
        CommandHandlersTest,
        CommandHandlersTest,
        ::testing::Values(
                std::make_tuple(std::make_shared<AddCommandHandler>(), Command::Value(1.0), Command::Value(2.0), Result(3.0)),
                std::make_tuple(std::make_shared<SubCommandHandler>(), Command::Value(3.0), Command::Value(2.0), Result(1.0)),
                std::make_tuple(std::make_shared<DivCommandHandler>(), Command::Value(1.0), Command::Value(2.0), Result(0.5)),
                std::make_tuple(std::make_shared<MulCommandHandler>(), Command::Value(5.0), Command::Value(2.0), Result(10.0)),

                std::make_tuple(std::make_shared<AbsCommandHandler>(), Command::Value(-1.0), Command::Value(NAN), Result(1.0)),
                std::make_tuple(std::make_shared<AbsCommandHandler>(), Command::Value(1.0), Command::Value(NAN), Result(1.0)),

                std::make_tuple(std::make_shared<SqrtCommandHandler>(), Command::Value(2.0), Command::Value(NAN), Result(std::sqrt(2))),

                std::make_tuple(std::make_shared<NegCommandHandler>(), Command::Value(1.0), Command::Value(NAN), Result(-1.0)),
                std::make_tuple(std::make_shared<NegCommandHandler>(), Command::Value(-1.0), Command::Value(NAN), Result(1.0)),

                std::make_tuple(std::make_shared<NotCommandHandler>(), Command::Value(true), Command::Value(NAN), Result(false)),
                std::make_tuple(std::make_shared<NotCommandHandler>(), Command::Value(false), Command::Value(NAN), Result(true)),

                std::make_tuple(std::make_shared<OrCommandHandler>(), Command::Value(true), Command::Value(true), Result(true)),
                std::make_tuple(std::make_shared<OrCommandHandler>(), Command::Value(true), Command::Value(false), Result(true)),
                std::make_tuple(std::make_shared<OrCommandHandler>(), Command::Value(false), Command::Value(true), Result(true)),
                std::make_tuple(std::make_shared<OrCommandHandler>(), Command::Value(false), Command::Value(false), Result(false)),

                std::make_tuple(std::make_shared<AndCommandHandler>(), Command::Value(true), Command::Value(true), Result(true)),
                std::make_tuple(std::make_shared<AndCommandHandler>(), Command::Value(true), Command::Value(false), Result(false)),
                std::make_tuple(std::make_shared<AndCommandHandler>(), Command::Value(false), Command::Value(true), Result(false)),
                std::make_tuple(std::make_shared<AndCommandHandler>(), Command::Value(false), Command::Value(false), Result(false)),

                std::make_tuple(std::make_shared<XorCommandHandler>(), Command::Value(true), Command::Value(true), Result(false)),
                std::make_tuple(std::make_shared<XorCommandHandler>(), Command::Value(true), Command::Value(false), Result(true)),
                std::make_tuple(std::make_shared<XorCommandHandler>(), Command::Value(false), Command::Value(true), Result(true)),
                std::make_tuple(std::make_shared<XorCommandHandler>(), Command::Value(false), Command::Value(false), Result(false)),

                std::make_tuple(std::make_shared<EqCommandHandler>(), Command::Value(1.0), Command::Value(2.0), Result(false)),
                std::make_tuple(std::make_shared<EqCommandHandler>(), Command::Value(1.0), Command::Value(1.0), Result(true)),

                std::make_tuple(std::make_shared<NeCommandHandler>(), Command::Value(1.0), Command::Value(2.0), Result(true)),
                std::make_tuple(std::make_shared<NeCommandHandler>(), Command::Value(1.0), Command::Value(1.0), Result(false)),

                std::make_tuple(std::make_shared<LtCommandHandler>(), Command::Value(1.0), Command::Value(2.0), Result(true)),
                std::make_tuple(std::make_shared<LtCommandHandler>(), Command::Value(1.0), Command::Value(1.0), Result(false)),
                std::make_tuple(std::make_shared<LtCommandHandler>(), Command::Value(2.0), Command::Value(1.0), Result(false)),

                std::make_tuple(std::make_shared<LeCommandHandler>(), Command::Value(1.0), Command::Value(2.0), Result(true)),
                std::make_tuple(std::make_shared<LeCommandHandler>(), Command::Value(1.0), Command::Value(1.0), Result(true)),
                std::make_tuple(std::make_shared<LeCommandHandler>(), Command::Value(2.0), Command::Value(1.0), Result(false)),

                std::make_tuple(std::make_shared<GtCommandHandler>(), Command::Value(2.0), Command::Value(1.0), Result(true)),
                std::make_tuple(std::make_shared<GtCommandHandler>(), Command::Value(1.0), Command::Value(1.0), Result(false)),
                std::make_tuple(std::make_shared<GtCommandHandler>(), Command::Value(1.0), Command::Value(2.0), Result(false)),

                std::make_tuple(std::make_shared<GeCommandHandler>(), Command::Value(2.0), Command::Value(1.0), Result(true)),
                std::make_tuple(std::make_shared<GeCommandHandler>(), Command::Value(1.0), Command::Value(1.0), Result(true)),
                std::make_tuple(std::make_shared<GeCommandHandler>(), Command::Value(1.0), Command::Value(2.0), Result(false))
        ));

/**
 * Тест проверяет, что реализация команды способна правильно обработать константные операнды.
 */
TEST_P(CommandHandlersTest, constants) {
    auto [handler, lhs, rhs, res] = GetParam();

    Command cmd;
    cmd.operation = handler->operation();
    cmd.lhsOperand.value = lhs;
    cmd.rhsOperand.value = rhs;
    cmd.resultRef = 1;

    EXPECT_CALL(memory, setValue(1, ResultIs(res)))
        .Times(1);

    CommandHandler::ExecuteArgs args { &cmd, &memory, nullptr, nullptr };
    EXPECT_NO_THROW({
        EXPECT_TRUE(handler->execute(args));
    });
}

/**
 * Тест проверяет, что реализация команды возвращает ЛОЖЬ, если значение левого операнда-ссылки не установлено.
 */
TEST_P(CommandHandlersTest, left_operand_is_reference_to_not_ready_value) {
    auto [handler, lhs, rhs, res] = GetParam();

    Command cmd;
    cmd.operation = handler->operation() | Command::Flags::LHS_OP_REF;
    cmd.lhsOperand.ref = 2;
    cmd.rhsOperand.value = rhs;
    cmd.resultRef = 1;

    EXPECT_CALL(memory, value(Command::Reference(2)))
            .WillOnce(Return(std::nullopt));

    CommandHandler::ExecuteArgs args { &cmd, &memory, nullptr, nullptr };
    EXPECT_NO_THROW({
        EXPECT_FALSE(handler->execute(args));
    });
}

/**
 * Тест проверяет, что реализация команды возвращает ИСТИНУ, если значение левого операнда-ссылки установлено,
 * а также записывает результат операции в память.
 */
TEST_P(CommandHandlersTest, left_operand_is_reference) {
    auto [handler, lhs, rhs, res] = GetParam();

    Command cmd;
    cmd.operation = handler->operation() | Command::Flags::LHS_OP_REF;
    cmd.lhsOperand.ref = 2;
    cmd.rhsOperand.value = rhs;
    cmd.resultRef = 1;

    EXPECT_CALL(memory, value(2))
            .WillOnce(Return(std::optional(Command::Value(lhs))));

    EXPECT_CALL(memory, setValue(1, ResultIs(res)))
            .Times(1);

    CommandHandler::ExecuteArgs args { &cmd, &memory, nullptr, nullptr };
    EXPECT_NO_THROW({
        EXPECT_TRUE(handler->execute(args));
    });
}

/**
 * Тест проверяет, что реализация команды возвращает ЛОЖЬ, если значение левого операнда-ссылки не установлено.
 *
 * Для унарных операций тест пропускается, т.к. унарные операции пользуются левым операндом для передачи аргумента.
 */
TEST_P(CommandHandlersTest, right_operand_is_reference_to_not_ready_value) {
    auto [handler, lhs, rhs, res] = GetParam();

    // Если операция унарная, то тест пропускается
    if (std::isnan(rhs.number)) {
        GTEST_SKIP();
    }

    Command cmd;
    cmd.operation = handler->operation() | Command::Flags::RHS_OP_REF;
    cmd.lhsOperand.value = lhs;
    cmd.rhsOperand.ref = 2;
    cmd.resultRef = 1;

    EXPECT_CALL(memory, value(Command::Reference(2)))
            .WillOnce(Return(std::nullopt));

    CommandHandler::ExecuteArgs args { &cmd, &memory, nullptr, nullptr };
    EXPECT_NO_THROW({
        EXPECT_FALSE(handler->execute(args));
    });
}

/**
 * Тест проверяет, что реализация команды возвращает ИСТИНУ, если значение правого операнда-ссылки установлено,
 * а также записывает результат операции в память.
 *
 * Для унарных операций тест пропускается, т.к. унарные операции пользуются левым операндом для передачи аргумента.
 */
TEST_P(CommandHandlersTest, right_operand_is_reference) {
    auto [handler, lhs, rhs, res] = GetParam();

    // Если операция унарная, то тест пропускается
    if (std::isnan(rhs.number)) {
        GTEST_SKIP();
    }

    Command cmd;
    cmd.operation = handler->operation() | Command::Flags::RHS_OP_REF;
    cmd.lhsOperand.value = lhs;
    cmd.rhsOperand.ref = 2;
    cmd.resultRef = 1;

    EXPECT_CALL(memory, value(2))
            .WillOnce(Return(std::optional(Command::Value(rhs))));

    EXPECT_CALL(memory, setValue(1, ResultIs(res)))
            .Times(1);

    CommandHandler::ExecuteArgs args { &cmd, &memory, nullptr, nullptr };
    EXPECT_NO_THROW({
        EXPECT_TRUE(handler->execute(args));
    });
}

TEST(CommandHandlersTest, LQT_command) {
    auto program = QVM_ASM_BEGIN(std::vector<Command> {})
        CMD_LQT(VAL(false), VAL(1.0), 1);
        CMD_LQT(VAL(true), VAL(2.0), 1);
    QVM_ASM_END;

    MockMemory memory;
    LqtCommandHandler handler;
    CancellationToken cancel;
    CommandHandler::ExecuteArgs args { &program[0], &memory, &cancel, nullptr };

    EXPECT_CALL(memory, setValue(1, ResultIs(1.0))).Times(0);
    EXPECT_NO_THROW({
        EXPECT_TRUE(handler.execute(args));
    });
    EXPECT_FALSE(cancel);

    args.cmd = &program[1];
    EXPECT_CALL(memory, setValue(1, ResultIs(2.0))).Times(1);
    EXPECT_NO_THROW({
        EXPECT_TRUE(handler.execute(args));
    });
    EXPECT_FALSE(cancel);
}

TEST(CommandHandlersTest, LQT_with_halt_if_true_flag) {
    auto program = QVM_ASM_BEGIN(std::vector<Command> {})
        CMD_LQT_HALT(VAL(false), VAL((uint32_t) 1));
        CMD_LQT_HALT(VAL(true), VAL((uint32_t) 2));
    QVM_ASM_END;

    MockMemory memory;
    LqtCommandHandler handler;
    CancellationToken cancel;
    MockStringVault stringVault;
    CommandHandler::ExecuteArgs args { &program[0], &memory, &cancel, &stringVault };
    std::string expectedString = "expected string";

    EXPECT_CALL(memory, value(_)).Times(0);
    EXPECT_CALL(memory, setValue(_, _)).Times(0);

    EXPECT_NO_THROW({
        EXPECT_TRUE(handler.execute(args));
    });
    EXPECT_FALSE(cancel);

    args.cmd = &program[1];
    EXPECT_CALL(stringVault, string(2)).WillRepeatedly(ReturnRef(expectedString));
    EXPECT_NO_THROW({
        EXPECT_TRUE(handler.execute(args));
    });
    EXPECT_TRUE(cancel);
    EXPECT_EQ(expectedString, cancel.reason());
}
