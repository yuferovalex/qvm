#ifndef VM_COMMAND_H
#define VM_COMMAND_H

#include <cstdint>

/**
 * Команда.
 */
struct Command {
    /**
     * Операции.
     */
    enum Operation : uint32_t {
        ADD  = 0x00000000,  /**< Сложение  */
        SUB  = 0x01000000,  /**< Вычитание */
        MUL  = 0x02000000,  /**< Умножение */
        DIV  = 0x03000000,  /**< Деление   */

        ABS  = 0x04000000,  /**< Модуль    */
        SQRT = 0x05000000,  /**< Корень    */
        NEG  = 0x06000000,  /**< Унарный минус (смена знака) */
        NOT  = 0x07000000,  /**< Логическое отрицание        */

        OR   = 0x08000000,  /**< Конъюнкция             */
        AND  = 0x09000000,  /**< Дизъюнкция             */
        XOR  = 0x0A000000,  /**< Исключающее ИЛИ        */

        EQ   = 0x0B000000,  /**< Сравнение на равенство */
        NE   = 0x0C000000,  /**< Не равно               */
        LT   = 0x0D000000,  /**< Меньше                 */
        LE   = 0x0E000000,  /**< Меньше или равно       */
        GT   = 0x0F000000,  /**< Больше                 */
        GE   = 0x10000000,  /**< Больше или равно       */

        LQT  = 0x11000000,  /**< Логический Q-терм      */

        COUNT   = (LQT >> (3 * 8)) + 1, /**< Кол-во операций */
    };

    /**
     * Флаги.
     */
    enum Flags : uint32_t {
        LHS_OP_REF = 0x00000001, /**< Левый операнд - ссылка  */
        RHS_OP_REF = 0x00000002, /**< Правый операнд - ссылка */
    };

    /**
     * Тип ссылки в памяти ВМ.
     */
    typedef uint64_t Reference;

    /**
     * Тип значения.
     */
    union Value {
        double number;
        bool   boolean;

        Value() = default;
        explicit Value(double number) : number(number) {}
        explicit Value(bool boolean) : boolean(boolean) {}
    };

    /**
     * Тип операнда.
     */
    union Operand {
        Reference ref = 0;
        Value value;

        Operand() = default;
        explicit Operand(Value value) : value(value) {}
        explicit Operand(Reference ref) : ref(ref) {}
    };

    uint32_t operation;
    uint32_t : 32;
    Operand lhsOperand;
    Operand rhsOperand;
    Reference resultRef;
};

// Команда должна помещаться в кеш-линию
static_assert(64 % sizeof(Command) == 0, "Command should place into cache-line");

#endif //VM_COMMAND_H
