#ifndef VM_DISPATCHERCONFIGIMPL_H
#define VM_DISPATCHERCONFIGIMPL_H

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
    const size_t DEFAULT_THREAD_COUNT = std::thread::hardware_concurrency() - 1;
    const size_t DEFAULT_BATCH_SIZE = 1'000'000 /** команд в пакете */;

    explicit DispatcherConfigImpl(Memory &memory, std::optional<size_t> batchSize = std::nullopt,
            std::optional<size_t> threadCount = std::nullopt);

    size_t batchSize() const final;

    size_t threadCount() const final;

    std::unique_ptr<Interpreter> createInterpreter() final;

private:
    Memory &m_memory;
    size_t m_batchSize;
    size_t m_threadCount;
};

#endif //VM_DISPATCHERCONFIGIMPL_H
