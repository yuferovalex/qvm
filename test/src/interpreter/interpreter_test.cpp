#include <memory>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "qvm/Command.h"
#include "interpreter/InterpreterImpl.h"
#include "interpreter/CommandHandler.h"
#include "memory/Memory.h"

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

    class MockCommandHandler : public CommandHandler {
    public:
        MOCK_CONST_METHOD0(operation, Command::Operation ());
        MOCK_METHOD2(execute, bool (Memory &memory, const Command &cmd));
    };

    class InterpreterTest : public ::testing::Test {
    protected:
        void SetUp() override {
            Test::SetUp();

            addCommandHandler = new MockCommandHandler();

            ON_CALL(*addCommandHandler, operation())
                .WillByDefault(Return(Command::Operation::ADD));

            subCommandHandler = new MockCommandHandler();

            ON_CALL(*subCommandHandler, operation())
                    .WillByDefault(Return(Command::Operation::SUB));

            InterpreterImpl::registerCommandHandler(std::unique_ptr<CommandHandler>(addCommandHandler));
            InterpreterImpl::registerCommandHandler(std::unique_ptr<CommandHandler>(subCommandHandler));

            memory = new MockMemory();
            interpreter = new InterpreterImpl(*memory);
        }

        void TearDown() override {
            Test::TearDown();

            delete interpreter;
            delete memory;
            InterpreterImpl::unregisterCommandHandler(Command::Operation::SUB);
            InterpreterImpl::unregisterCommandHandler(Command::Operation::ADD);
        }

        MockMemory *memory = nullptr;
        InterpreterImpl *interpreter = nullptr;
        MockCommandHandler *addCommandHandler = nullptr;
        MockCommandHandler *subCommandHandler = nullptr;
    };
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

        EXPECT_CALL(*subCommandHandler, execute(Ref(*memory),
                Field(&Command::operation, Command::Operation::SUB | Command::LHS_OP_REF)))
                .WillOnce(Return(false));

        EXPECT_CALL(*addCommandHandler, execute(Ref(*memory), Field(&Command::operation, Command::Operation::ADD)))
                .WillOnce(Return(true));

        EXPECT_CALL(*subCommandHandler, execute(Ref(*memory),
                Field(&Command::operation, Command::Operation::SUB | Command::LHS_OP_REF)))
                .WillOnce(Return(true));
    }

    interpreter->interpret(std::move(program));
}
