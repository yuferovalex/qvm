#ifndef QVM_DISPATCHERCONFIGIMPL_H
#define QVM_DISPATCHERCONFIGIMPL_H

#include <cstdint>
#include <thread>
#include <optional>

#include "dispatcher/DispatcherConfig.h"

class Memory;

/**
 * Реализация конфигурации диспетчера ВМ.
 */
class DispatcherConfigImpl final : public DispatcherConfig {
public:
    static const size_t DEFAULT_THREAD_COUNT;
    static const size_t DEFAULT_BATCH_SIZE;

    explicit DispatcherConfigImpl(Memory &memory, size_t batchSize = DEFAULT_BATCH_SIZE,
            size_t threadCount = DEFAULT_THREAD_COUNT);

    size_t batchSize() const final;

    size_t threadCount() const final;

    std::unique_ptr<Interpreter> createInterpreter() final;

private:
    Memory &m_memory;
    size_t m_batchSize;
    size_t m_threadCount;
};

#endif //VM_DISPATCHERCONFIGIMPL_H
