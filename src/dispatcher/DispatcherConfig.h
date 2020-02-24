#ifndef VM_DISPATCHERCONFIG_H
#define VM_DISPATCHERCONFIG_H

#include <cstdint>
#include <memory>

#include "interpreter/Interpreter.h"

/**
 * Интерфейс конфигурации диспетчера ВМ.
 */
class DispatcherConfig {
public:
    virtual ~DispatcherConfig() = default;

    /**
     * Кол-во операций, отдаваемых исполнителям при обращении.
     */
    [[nodiscard]]
    virtual size_t batchSize() const = 0;

    /**
     * Кол-во потоков.
     */
    [[nodiscard]]
    virtual size_t threadCount() const = 0;

    /**
     * Фабрика интерпретаторов.
     *
     * @return Созданный интерпретатор.
     */
    [[nodiscard]]
    virtual std::unique_ptr<Interpreter> createInterpreter() = 0;
};

#endif //VM_DISPATCHERCONFIG_H
