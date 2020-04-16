#ifndef VM_COMMANDHANDLER_H
#define VM_COMMANDHANDLER_H

#include "interpreter/Command.h"

class Memory;

/**
 * Интерфейс обработчика команды интерпретатора ВМ.
 */
class CommandHandler {
public:
    virtual ~CommandHandler() = default;

    /**
     * Операция, за которую отвечает обработчик.
     */
    virtual Command::Operation operation() const = 0;

    /**
     * Выполнить команду.
     *
     * @param memory Память ВМ.
     * @param cmd    Команда, которую необходимо выполнить.
     * @return Возвращает ИСТИНУ, если все необходимые аргументы были готовы к моменту исполнения, ЛОЖЬ - иначе.
     */
    virtual bool execute(Memory &memory, const Command &cmd) = 0;
};

#endif //VM_COMMANDHANDLER_H
