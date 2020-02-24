#ifndef VM_SHAREDMEMORY_H
#define VM_SHAREDMEMORY_H

#include <atomic>
#include <vector>
#include <mutex>

#include "memory/Memory.h"

/**
 * Реализация общей памяти ВМ.
 */
class SharedMemory final : public Memory {
public:
    /**
     * Конструктор.
     *
     * @param allocated Кол-во аллоцированных значений.
     */
    explicit SharedMemory(size_t allocated);

    /**
     * Получить значение по ссылке.
     *
     * @param ref ссылка на значение.
     * @return Возвращает значение хранящееся по этой ссылке.
     */
    [[nodiscard]]
    std::optional<Command::Value> value(Command::Reference ref) const final;

    /**
     * Установить значение по ссылке.
     *
     * @param ref ссылка на значение.
     * @param value устанавливаемое значение.
     */
    void setValue(Command::Reference ref, Command::Value value) final;

private:
    struct Cell {
        std::atomic_bool set = false;
        Command::Value value;
    };

    std::vector<Cell> m_vault;
};


#endif //VM_SHAREDMEMORY_H
