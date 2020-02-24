#ifndef VM_INTERPRETERIMPL_H
#define VM_INTERPRETERIMPL_H

#include <array>
#include <memory>

#include "interpreter/Interpreter.h"

class Memory;
class CommandHandler;

/**
 * Реализация интерпретатора байт-кода ВМ.
 */
class InterpreterImpl : public Interpreter {
public:
    explicit InterpreterImpl(Memory &memory);
    void interpret(CommandQueue &&commands) final;

    /**
     * Регистрация обработчика команд.
     *
     * @param handler Объект обработчика.
     */
    static void registerCommandHandler(std::unique_ptr<CommandHandler> &&handler);

    /**
     * Удаление зарегистрированного обработчика команд.
     *
     * @param operation Операция, обработчик которой необходимо удалить.
     */
    static void unregisterCommandHandler(Command::Operation operation);

private:
    static std::array<std::unique_ptr<CommandHandler>, Command::Operation::COUNT> m_commands;

    Memory &m_memory;
};

#endif //VM_INTERPRETERIMPL_H
