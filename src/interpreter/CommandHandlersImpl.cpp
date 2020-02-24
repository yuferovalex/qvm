#include "interpreter/CommandHandlersImpl.h"
#include "interpreter/InterpreterImpl.h"
#include "memory/Memory.h"

#define REGISTER_HANDLER(handler) \
    InterpreterImpl::registerCommandHandler(std::make_unique< handler >())

void registerCommandHandlers() {
    REGISTER_HANDLER(AddCommandHandler);
    REGISTER_HANDLER(SubCommandHandler);
    REGISTER_HANDLER(MulCommandHandler);
    REGISTER_HANDLER(DivCommandHandler);
    REGISTER_HANDLER(AbsCommandHandler);
    REGISTER_HANDLER(SqrtCommandHandler);
    REGISTER_HANDLER(NegCommandHandler);
    REGISTER_HANDLER(NotCommandHandler);
    REGISTER_HANDLER(OrCommandHandler);
    REGISTER_HANDLER(AndCommandHandler);
    REGISTER_HANDLER(XorCommandHandler);
    REGISTER_HANDLER(EqCommandHandler);
    REGISTER_HANDLER(NeCommandHandler);
    REGISTER_HANDLER(LtCommandHandler);
    REGISTER_HANDLER(LeCommandHandler);
    REGISTER_HANDLER(GtCommandHandler);
    REGISTER_HANDLER(GeCommandHandler);
}

template <typename Impl>
bool executeBinaryOperation(Memory &memory, const Command &command, Impl &&impl);

template <typename Impl>
bool executeUnaryOperation(Memory &memory, const Command &command, Impl &&impl);


bool AddCommandHandler::execute(Memory &memory, const Command &cmd) {
    return executeBinaryOperation(memory, cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.number + rhs.number };
    });
}

bool SubCommandHandler::execute(Memory &memory, const Command &cmd) {
    return executeBinaryOperation(memory, cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.number - rhs.number };
    });
}

bool MulCommandHandler::execute(Memory &memory, const Command &cmd) {
    return executeBinaryOperation(memory, cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.number * rhs.number };
    });
}

bool DivCommandHandler::execute(Memory &memory, const Command &cmd) {
    return executeBinaryOperation(memory, cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.number / rhs.number };
    });
}

bool AbsCommandHandler::execute(Memory &memory, const Command &cmd) {
    return executeUnaryOperation(memory, cmd, [](Command::Value lhs) {
        return Command::Value { std::abs(lhs.number) };
    });
}

bool SqrtCommandHandler::execute(Memory &memory, const Command &cmd) {
    return executeUnaryOperation(memory, cmd, [](Command::Value lhs) {
        return Command::Value { std::sqrt(lhs.number) };
    });
}

bool NegCommandHandler::execute(Memory &memory, const Command &cmd) {
    return executeUnaryOperation(memory, cmd, [](Command::Value lhs) {
        return Command::Value { - lhs.number };
    });
}

bool NotCommandHandler::execute(Memory &memory, const Command &cmd) {
    return executeUnaryOperation(memory, cmd, [](Command::Value lhs) {
        return Command::Value { ! lhs.boolean };
    });
}

bool OrCommandHandler::execute(Memory &memory, const Command &cmd) {
    return executeBinaryOperation(memory, cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.boolean || rhs.boolean };
    });
}

bool AndCommandHandler::execute(Memory &memory, const Command &cmd) {
    return executeBinaryOperation(memory, cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.boolean && rhs.boolean };
    });
}

bool XorCommandHandler::execute(Memory &memory, const Command &cmd) {
    return executeBinaryOperation(memory, cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value {
                (!lhs.boolean && rhs.boolean) || (lhs.boolean && !rhs.boolean)
        };
    });
}

bool EqCommandHandler::execute(Memory &memory, const Command &cmd) {
    return executeBinaryOperation(memory, cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value {
            std::abs(lhs.number - rhs.number) <= std::numeric_limits<decltype(Command::Value::number)>::epsilon()
        };
    });
}

bool NeCommandHandler::execute(Memory &memory, const Command &cmd) {
    return executeBinaryOperation(memory, cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value {
                std::abs(lhs.number - rhs.number) > std::numeric_limits<decltype(Command::Value::number)>::epsilon()
        };
    });
}

bool LtCommandHandler::execute(Memory &memory, const Command &cmd) {
    return executeBinaryOperation(memory, cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.number < rhs.number };
    });
}

bool LeCommandHandler::execute(Memory &memory, const Command &cmd) {
    return executeBinaryOperation(memory, cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.number <= rhs.number };
    });
}

bool GtCommandHandler::execute(Memory &memory, const Command &cmd) {
    return executeBinaryOperation(memory, cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.number > rhs.number };
    });
}

bool GeCommandHandler::execute(Memory &memory, const Command &cmd) {
    return executeBinaryOperation(memory, cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.number >= rhs.number };
    });
}

template <typename Impl>
bool executeUnaryOperation(Memory &memory, const Command &command, Impl &&impl) {
    Command::Value operandValue = command.lhsOperand.value;
    if (command.operation & Command::Flags::LHS_OP_REF) {
        auto value = memory.value(command.lhsOperand.ref);
        if (!value) {
            return false;
        }
        operandValue = *value;
    }

    Command::Value result = impl(operandValue);
    memory.setValue(command.resultRef, result);
    return true;
}

template <typename Impl>
bool executeBinaryOperation(Memory &memory, const Command &command, Impl &&impl) {
    Command::Value lhsValue = command.lhsOperand.value;
    if (command.operation & Command::Flags::LHS_OP_REF) {
        auto value = memory.value(command.lhsOperand.ref);
        if (!value) {
            return false;
        }
        lhsValue = *value;
    }

    Command::Value rhsValue = command.rhsOperand.value;
    if (command.operation & Command::Flags::RHS_OP_REF) {
        auto value = memory.value(command.rhsOperand.ref);
        if (!value) {
            return false;
        }
        rhsValue = *value;
    }

    Command::Value result = impl(lhsValue, rhsValue);
    memory.setValue(command.resultRef, result);
    return true;
}
