#include <memory>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "qvm/Command.h"
#include "interpreter/InterpreterImpl.h"
#include "interpreter/CommandHandler.h"
#include "memory/Memory.h"
#include "memory/StringVault.h"
#include "utils/CancellationToken.h"

#include "test-utils/PseudoAssemblerLanguage.h"

using ::testing::Return;
using ::testing::Field;
using ::testing::Ref;

namespace {
    class MockMemory : public Memory {
    public:
        MOCK_CONST_METHOD1(value, std::optional<Command::Value> (Command::Reference ref));
        MOCK_METHOD2(setValue, void (Command::Reference ref, Command::Value value));
    };

    class MockStringVault : public StringVault {
    public:
        MOCK_CONST_METHOD1(string, const std::string &(uint32_t id));
        MOCK_METHOD1(addString, uint32_t (const std::string &string));
    };

    class MockCommandHandler : public CommandHandler {
    public:
        MOCK_CONST_METHOD0(operation, Command::Operation ());
        MOCK_METHOD1(execute, bool (CommandHandler::ExecuteArgs &args));
    };

    class InterpreterTest : public ::testing::Test {
    protected:
        void SetUp() override {
            Test::SetUp();

            addCommandHandler = new MockCommandHandler();
            subCommandHandler = new MockCommandHandler();

            EXPECT_CALL(*addCommandHandler, operation())
                    .WillOnce(Return(Command::Operation::ADD));

            EXPECT_CALL(*subCommandHandler, operation())
                    .WillOnce(Return(Command::Operation::SUB));

            InterpreterImpl::registerCommandHandler(std::unique_ptr<CommandHandler>(addCommandHandler));
            InterpreterImpl::registerCommandHandler(std::unique_ptr<CommandHandler>(subCommandHandler));

            memory = new MockMemory();
            stringVault = new MockStringVault();
            interpreter = new InterpreterImpl(*memory, *stringVault);
        }

        void TearDown() override {
            Test::TearDown();

            delete interpreter;
            delete stringVault;
            delete memory;
            InterpreterImpl::unregisterCommandHandler(Command::Operation::SUB);
            InterpreterImpl::unregisterCommandHandler(Command::Operation::ADD);
        }

        MockMemory *memory = nullptr;
        MockStringVault *stringVault = nullptr;
        InterpreterImpl *interpreter = nullptr;
        MockCommandHandler *addCommandHandler = nullptr;
        MockCommandHandler *subCommandHandler = nullptr;
    };
}

MATCHER_P(OperationIs, expectedOperation, "operation is") {
    return arg.cmd->operation == expectedOperation;
}

/**
 * Тест проверяет, что реализация интерпретатора байт-кода ВМ обрабатывает команды в порядке очереди, а если
 * команда ещё не готова к выполнению (её аргументы не вычислены), то она ставится в конец очереди.
 */
TEST_F(InterpreterTest, should_execute_commands_in_queue_order) {
    auto program = QVM_ASM_BEGIN(InterpreterImpl::CommandQueue{2})
        CMD_SUB(REF(2), VAL(2.0), 1);
        CMD_ADD(VAL(1.0), VAL(3.0), 2);
    QVM_ASM_END;

    {
        ::testing::InSequence sequence;

        EXPECT_CALL(*subCommandHandler, execute(
                OperationIs(Command::Operation::SUB | Command::LHS_OP_REF)))
                .WillOnce(Return(false));

        EXPECT_CALL(*addCommandHandler, execute(OperationIs(Command::Operation::ADD)))
                .WillOnce(Return(true));

        EXPECT_CALL(*subCommandHandler, execute(
                OperationIs(Command::Operation::SUB | Command::LHS_OP_REF)))
                .WillOnce(Return(true));
    }

    CancellationToken cancel;
    interpreter->interpret(std::move(program), cancel);
}
