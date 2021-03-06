#ifndef QVM_DISPATCHERCONFIG_H
#define QVM_DISPATCHERCONFIG_H

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
    virtual size_t batchSize() const = 0;

    /**
     * Кол-во потоков.
     */
    virtual size_t threadCount() const = 0;

    /**
     * Фабрика интерпретаторов.
     *
     * @return Созданный интерпретатор.
     */
    virtual std::unique_ptr<Interpreter> createInterpreter() = 0;
};

#endif //QVM_DISPATCHERCONFIG_H
