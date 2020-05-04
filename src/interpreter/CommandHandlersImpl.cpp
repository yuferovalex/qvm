#include "interpreter/CommandHandlersImpl.h"
#include "interpreter/InterpreterImpl.h"
#include "memory/Memory.h"
#include "memory/StringVault.h"
#include "utils/CancellationToken.h"

#define REGISTER_HANDLER(handler) \
    InterpreterImpl::registerCommandHandler(std::make_unique< handler >())

void registerCommandHandlers() {
    static bool isInit = false;
    if (isInit) {
        return;
    }
    isInit = true;

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
    REGISTER_HANDLER(LqtCommandHandler);
}

template <typename Impl>
bool executeBinaryOperation(Memory &memory, const Command &command, Impl &&impl);

template <typename Impl>
bool executeUnaryOperation(Memory &memory, const Command &command, Impl &&impl);


bool AddCommandHandler::execute(ExecuteArgs &args) {
    return executeBinaryOperation(*args.memory, *args.cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.number + rhs.number };
    });
}

bool SubCommandHandler::execute(ExecuteArgs &args) {
    return executeBinaryOperation(*args.memory, *args.cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.number - rhs.number };
    });
}

bool MulCommandHandler::execute(ExecuteArgs &args) {
    return executeBinaryOperation(*args.memory, *args.cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.number * rhs.number };
    });
}

bool DivCommandHandler::execute(ExecuteArgs &args) {
    return executeBinaryOperation(*args.memory, *args.cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.number / rhs.number };
    });
}

bool AbsCommandHandler::execute(ExecuteArgs &args) {
    return executeUnaryOperation(*args.memory, *args.cmd, [](Command::Value lhs) {
        return Command::Value { std::abs(lhs.number) };
    });
}

bool SqrtCommandHandler::execute(ExecuteArgs &args) {
    return executeUnaryOperation(*args.memory, *args.cmd, [](Command::Value lhs) {
        return Command::Value { std::sqrt(lhs.number) };
    });
}

bool NegCommandHandler::execute(ExecuteArgs &args) {
    return executeUnaryOperation(*args.memory, *args.cmd, [](Command::Value lhs) {
        return Command::Value { - lhs.number };
    });
}

bool NotCommandHandler::execute(ExecuteArgs &args) {
    return executeUnaryOperation(*args.memory, *args.cmd, [](Command::Value lhs) {
        return Command::Value { ! lhs.boolean };
    });
}

bool OrCommandHandler::execute(ExecuteArgs &args) {
    return executeBinaryOperation(*args.memory, *args.cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.boolean || rhs.boolean };
    });
}

bool AndCommandHandler::execute(ExecuteArgs &args) {
    return executeBinaryOperation(*args.memory, *args.cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.boolean && rhs.boolean };
    });
}

bool XorCommandHandler::execute(ExecuteArgs &args) {
    return executeBinaryOperation(*args.memory, *args.cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value {
                (!lhs.boolean && rhs.boolean) || (lhs.boolean && !rhs.boolean)
        };
    });
}

bool EqCommandHandler::execute(ExecuteArgs &args) {
    return executeBinaryOperation(*args.memory, *args.cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value {
            std::abs(lhs.number - rhs.number) <= std::numeric_limits<decltype(Command::Value::number)>::epsilon()
        };
    });
}

bool NeCommandHandler::execute(ExecuteArgs &args) {
    return executeBinaryOperation(*args.memory, *args.cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value {
                std::abs(lhs.number - rhs.number) > std::numeric_limits<decltype(Command::Value::number)>::epsilon()
        };
    });
}

bool LtCommandHandler::execute(ExecuteArgs &args) {
    return executeBinaryOperation(*args.memory, *args.cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.number < rhs.number };
    });
}

bool LeCommandHandler::execute(ExecuteArgs &args) {
    return executeBinaryOperation(*args.memory, *args.cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.number <= rhs.number };
    });
}

bool GtCommandHandler::execute(ExecuteArgs &args) {
    return executeBinaryOperation(*args.memory, *args.cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.number > rhs.number };
    });
}

bool GeCommandHandler::execute(ExecuteArgs &args) {
    return executeBinaryOperation(*args.memory, *args.cmd, [](Command::Value lhs, Command::Value rhs) {
        return Command::Value { lhs.number >= rhs.number };
    });
}

inline bool extractOperand(Memory &memory, const Command &command, Command::Value &lhs) {
    bool result = true;
    if (BOOST_LIKELY(command.operation & Command::Flags::LHS_OP_REF)) {
        if (auto value = memory.value(command.lhsOperand.ref)) {
            lhs = *value;
            result = true;
        } else {
            result = false;
        }
    }
    return result;
}

inline bool extractOperand(Memory &memory, const Command &command, Command::Value &lhs, Command::Value &rhs) {
    bool result = extractOperand(memory, command, lhs);
    if (BOOST_LIKELY(command.operation & Command::Flags::RHS_OP_REF)) {
        if (auto value = memory.value(command.rhsOperand.ref)) {
            rhs = *value;
            result = true;
        } else {
            result = false;
        }
    }
    return result;
}

template <typename Impl>
inline bool executeUnaryOperation(Memory &memory, const Command &command, Impl &&impl) {
    auto operandValue = command.lhsOperand.value;
    auto operandIsReady = extractOperand(memory, command, operandValue);
    if (operandIsReady) {
        Command::Value result = impl(operandValue);
        memory.setValue(command.resultRef, result);
    }
    return operandIsReady;
}

template <typename Impl>
inline bool executeBinaryOperation(Memory &memory, const Command &command, Impl &&impl) {
    auto lhsValue = command.lhsOperand.value;
    auto rhsValue = command.rhsOperand.value;
    auto operandsAreReady = extractOperand(memory, command, lhsValue, rhsValue);
    if (operandsAreReady) {
        auto result = impl(lhsValue, rhsValue);
        memory.setValue(command.resultRef, result);
    }
    return operandsAreReady;
}

bool LqtCommandHandler::execute(ExecuteArgs &args) {
    const auto &command = *args.cmd;
    auto &memory = *args.memory;
    auto lhsValue = command.lhsOperand.value;
    auto rhsValue = command.rhsOperand.value;
    auto operandsAreReady = extractOperand(memory, command, lhsValue, rhsValue);
    bool result = operandsAreReady;
    if (operandsAreReady) {
        if (lhsValue.boolean) {
            if (BOOST_UNLIKELY(command.operation & Command::Flags::HALT_IF_TRUE)) {
                auto reason = args.stringVault->string(rhsValue.stringIndex);
                args.cancel->cancel(reason);
            } else {
                memory.setValue(command.resultRef, rhsValue);
            }
        }
        result = true;
    }
    return result;
}

