#ifndef QVM_COMMANDHANDLER_H
#define QVM_COMMANDHANDLER_H

#include "qvm/Command.h"

class Memory;
class CancellationToken;
class StringVault;

/**
 * Интерфейс обработчика команды интерпретатора ВМ.
 */
class CommandHandler {
public:
    virtual ~CommandHandler() = default;

    /**
     * Аргументы для исполнителей команд.
     */
    struct ExecuteArgs {
        /**
         * Команда, которую необходимо выполнить.
         */
        const Command *cmd;

        /**
         * Память ВМ.
         */
        Memory *memory;

        /**
         * Токен отмены выполнения программы.
         */
        CancellationToken *cancel;

        /**
         * Хранилище строк.
         */
        StringVault *stringVault;
    };

    /**
     * Операция, за которую отвечает обработчик.
     */
    virtual Command::Operation operation() const = 0;

    /**
     * Выполнить команду.
     *
     * @param ExecuteArgs аргументы для выполнения.
     * @return Возвращает ИСТИНУ, если все необходимые аргументы были готовы
     *         к моменту исполнения, ЛОЖЬ - иначе.
     */
    virtual bool execute(ExecuteArgs &args) = 0;
};

#endif //QVM_COMMANDHANDLER_H
