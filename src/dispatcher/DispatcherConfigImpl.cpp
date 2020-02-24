#include "dispatcher/DispatcherConfigImpl.h"
#include "interpreter/InterpreterImpl.h"

DispatcherConfigImpl::DispatcherConfigImpl(Memory &memory, std::optional<size_t> batchSize,
                                           std::optional<size_t> threadCount)
        : m_memory(memory)
        , m_batchSize(batchSize.value_or(DEFAULT_BATCH_SIZE))
        , m_threadCount(threadCount.value_or(DEFAULT_THREAD_COUNT))
{}

size_t DispatcherConfigImpl::batchSize() const {
    return m_batchSize;
}

size_t DispatcherConfigImpl::threadCount() const {
    return m_threadCount;
}

std::unique_ptr<Interpreter> DispatcherConfigImpl::createInterpreter() {
    return std::make_unique<InterpreterImpl>(m_memory);
}
