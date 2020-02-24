#include "interpreter/CommandHandler.h"
#include "interpreter/InterpreterImpl.h"

std::array<std::unique_ptr<CommandHandler>, Command::Operation::COUNT> InterpreterImpl::m_commands {};

void InterpreterImpl::registerCommandHandler(std::unique_ptr<CommandHandler> &&handler) {
    auto index = handler->operation() >> 8 * 3;
    m_commands[index] = std::move(handler);
}

void InterpreterImpl::unregisterCommandHandler(Command::Operation operation) {
    auto index = operation >> 8 * 3;
    m_commands[index].reset();
}

InterpreterImpl::InterpreterImpl(Memory &memory)
    : m_memory(memory)
{}

void InterpreterImpl::interpret(CommandQueue &&commands) {
    while (!commands.empty()) {
        auto &&currentCommand = commands.front();
        commands.pop_front();

        auto operation = currentCommand.operation >> 8 * 3;
        auto result = m_commands[operation]->execute(m_memory, currentCommand);
        if (!result) {
            commands.push_back(currentCommand);
        }
    }
}
