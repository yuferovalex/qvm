#include "interpreter/CommandHandler.h"
#include "interpreter/InterpreterImpl.h"
#include "utils/CancellationToken.h"

std::array<std::unique_ptr<CommandHandler>, Command::Operation::COUNT> InterpreterImpl::m_commands{};

static inline uint32_t cmdIndex(uint32_t operation) {
    return operation >> (8u * 3u);
}

void InterpreterImpl::registerCommandHandler(std::unique_ptr<CommandHandler> &&handler) {
    auto index = cmdIndex(handler->operation());
    m_commands[index] = std::move(handler);
}

void InterpreterImpl::unregisterCommandHandler(Command::Operation operation) {
    auto index = cmdIndex(operation);
    m_commands[index].reset();
}

InterpreterImpl::InterpreterImpl(Memory &memory, StringVault &stringVault)
        : m_memory(memory), m_stringVault(stringVault) {}

void InterpreterImpl::interpret(CommandQueue &&commands, CancellationToken &cancel) {
    CommandHandler::ExecuteArgs args{nullptr, &m_memory, &cancel, &m_stringVault};
    while (!cancel && !commands.empty()) {
        auto &&currentCommand = commands.front();
        commands.pop_front();

        auto operation = cmdIndex(currentCommand.operation);
        args.cmd = &currentCommand;
        auto result = m_commands[operation]->execute(args);
        if (!result) {
            commands.push_back(currentCommand);
        }
    }
}
