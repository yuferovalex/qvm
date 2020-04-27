#include "dispatcher/DispatcherConfigImpl.h"
#include "interpreter/InterpreterImpl.h"

const size_t DispatcherConfigImpl::DEFAULT_THREAD_COUNT = std::thread::hardware_concurrency() - 1;
const size_t DispatcherConfigImpl::DEFAULT_BATCH_SIZE = 1'000'000 /** команд в пакете */;

DispatcherConfigImpl::DispatcherConfigImpl(Memory &memory, size_t batchSize, size_t threadCount)
        : m_memory(memory)
        , m_batchSize(batchSize)
        , m_threadCount(threadCount)
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
