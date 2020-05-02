#ifndef VM_COMMANDHANDLERSIMPL_H
#define VM_COMMANDHANDLERSIMPL_H

#include <cmath>

#include "qvm/Command.h"
#include "interpreter/CommandHandler.h"

/**
 * Регистрация обработчиков команд в интерпретаторе.
 */
void registerCommandHandlers();

/**
 * Обработчик команды сложения чисел.
 */
class AddCommandHandler final : public CommandHandler {
public:
    Command::Operation operation() const final { return Command::Operation::ADD; }

    bool execute(Memory &memory, const Command &cmd) final;
};


/**
 * Обработчик команды вычитания чисел.
 */
class SubCommandHandler final : public CommandHandler {
public:
    Command::Operation operation() const final { return Command::Operation::SUB; }

    bool execute(Memory &memory, const Command &cmd) final;
};


/**
 * Обработчик команды умножения чисел.
 */
class MulCommandHandler final : public CommandHandler {
public:
    Command::Operation operation() const final { return Command::Operation::MUL; }

    bool execute(Memory &memory, const Command &cmd) final;
};


/**
 * Обработчик команды деления чисел.
 */
class DivCommandHandler final : public CommandHandler {
public:
    Command::Operation operation() const final { return Command::Operation::DIV; }

    bool execute(Memory &memory, const Command &cmd) final;
};


/**
 * Обработчик команды взятия модуля от числа.
 */
class AbsCommandHandler final : public CommandHandler {
public:
    Command::Operation operation() const final { return Command::Operation::ABS; }

    bool execute(Memory &memory, const Command &cmd) final;
};


/**
 * Обработчик команды взятия квадратного корня от числа.
 */
class SqrtCommandHandler final : public CommandHandler {
public:
    Command::Operation operation() const final { return Command::Operation::SQRT; }

    bool execute(Memory &memory, const Command &cmd) final;
};


/**
 * Обработчик команды смены знака числа.
 */
class NegCommandHandler final : public CommandHandler {
public:
    Command::Operation operation() const final { return Command::Operation::NEG; }

    bool execute(Memory &memory, const Command &cmd) final;
};


/**
 * Обработчик команды логического отрицания.
 */
class NotCommandHandler final : public CommandHandler {
public:
    Command::Operation operation() const final { return Command::Operation::NOT; }

    bool execute(Memory &memory, const Command &cmd) final;
};


/**
 * Обработчик команды логического сложения (дизъюнкции).
 */
class OrCommandHandler final : public CommandHandler {
public:
    Command::Operation operation() const final { return Command::Operation::OR; }

    bool execute(Memory &memory, const Command &cmd) final;
};


/**
 * Обработчик команды логического умножения (конъюнкции).
 */
class AndCommandHandler final : public CommandHandler {
public:
    Command::Operation operation() const final { return Command::Operation::AND; }

    bool execute(Memory &memory, const Command &cmd) final;
};


/**
 * Обработчик команды логического сложения по модулю 2 (исключающее ИЛИ).
 */
class XorCommandHandler final : public CommandHandler {
public:
    Command::Operation operation() const final { return Command::Operation::XOR; }

    bool execute(Memory &memory, const Command &cmd) final;
};


/**
 * Обработчик команды сравнения чисел на равенство.
 */
class EqCommandHandler final : public CommandHandler {
public:
    Command::Operation operation() const final { return Command::Operation::EQ; }

    bool execute(Memory &memory, const Command &cmd) final;
};


/**
 * Обработчик команды сравнения чисел на неравенство.
 */
class NeCommandHandler final : public CommandHandler {
public:
    Command::Operation operation() const final { return Command::Operation::NE; }

    bool execute(Memory &memory, const Command &cmd) final;
};


/**
 * Обработчик команды "меньше".
 */
class LtCommandHandler final : public CommandHandler {
public:
    Command::Operation operation() const final { return Command::Operation::LT; }

    bool execute(Memory &memory, const Command &cmd) final;
};


/**
 * Обработчик команды "меньше или равно".
 */
class LeCommandHandler final : public CommandHandler {
public:
    Command::Operation operation() const final { return Command::Operation::LE; }

    bool execute(Memory &memory, const Command &cmd) final;
};


/**
 * Обработчик команды "больше".
 */
class GtCommandHandler final : public CommandHandler {
public:
    Command::Operation operation() const final { return Command::Operation::GT; }

    bool execute(Memory &memory, const Command &cmd) final;
};


/**
 * Обработчик команды "больше или равно".
 */
class GeCommandHandler final : public CommandHandler {
public:
    Command::Operation operation() const final { return Command::Operation::GE; }

    bool execute(Memory &memory, const Command &cmd) final;
};

#endif //VM_COMMANDHANDLERSIMPL_H
