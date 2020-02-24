#ifndef VM_INTERPRETER_H
#define VM_INTERPRETER_H

#include <boost/circular_buffer.hpp>

#include "interpreter/Command.h"

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
    virtual void interpret(CommandQueue &&commands) = 0;
};

#endif //VM_INTERPRETER_H
