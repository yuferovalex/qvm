#ifndef QVM_INTERPRETERIMPL_H
#define QVM_INTERPRETERIMPL_H

#include <array>
#include <memory>

#include "interpreter/Interpreter.h"

class Memory;
class CommandHandler;
class StringVault;

/**
 * Реализация интерпретатора байт-кода ВМ.
 */
class InterpreterImpl : public Interpreter {
public:
    InterpreterImpl(Memory &memory, StringVault &stringVault);
    void interpret(CommandQueue &&commands, CancellationToken &cancel) final;

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
    StringVault &m_stringVault;
};

#endif //QVM_INTERPRETERIMPL_H
