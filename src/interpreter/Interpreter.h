#ifndef QVM_INTERPRETER_H
#define QVM_INTERPRETER_H

#include <boost/circular_buffer.hpp>

#include "qvm/Command.h"

class CancellationToken;

/**
 * Интерфейс интерпретатора байт кода ВМ.
 */
class Interpreter {
public:
    virtual ~Interpreter() = default;

    /**
     * Тип очереди команд интерпретатора.
     */
    typedef boost::circular_buffer<Command> CommandQueue;

    /**
     * Выполнить интерпретацию команд в очереди.
     *
     * @param commands Очередь команд.
     */
    virtual void interpret(CommandQueue &&commands, CancellationToken &cancel) = 0;
};

#endif //QVM_INTERPRETER_H
