#ifndef VM_MEMORY_H
#define VM_MEMORY_H

#include <optional>

#include "qvm/Command.h"

class Memory {
public:
    virtual ~Memory() = default;

    /**
     * Получить значение по ссылке.
     *
     * @param ref ссылка на значение.
     * @return Возвращает значение хранящееся по этой ссылке.
     */
    virtual std::optional<Command::Value> value(Command::Reference ref) const = 0;

    /**
     * Установить значение по ссылке.
     *
     * @param ref ссылка на значение.
     * @param value устанавливаемое значение.
     */
    virtual void setValue(Command::Reference ref, Command::Value value) = 0;
};

#endif //VM_MEMORY_H
