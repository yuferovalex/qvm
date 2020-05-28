#include "dispatcher/Dispatcher.h"

Dispatcher::Dispatcher(DispatcherConfig &config)
    : m_config(config)
    , m_dispatchingFinished(false)
{}

bool Dispatcher::wasCanceled() const {
    return (bool) m_cancel;
}

std::string Dispatcher::cancelReason() const {
    return m_cancel.reason();
}

std::vector<std::thread> Dispatcher::createThreads() {
    m_dispatchingFinished = false;
    size_t threadCount = m_config.threadCount();
    std::vector<std::thread> threads(threadCount);
    for (size_t i = 0; i < threadCount; ++i) {
        auto thread = std::thread([&]() {
            auto interpreter = m_config.createInterpreter();
            while (handleNextBatch(*interpreter));
        });

        threads.emplace_back(std::move(thread));
    }
    return threads;
}

void Dispatcher::waitForAll(std::vector<std::thread> &&threads) {
    for (auto &&thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

bool Dispatcher::handleNextBatch(Interpreter &interpreter) {
    Batch queue;
    {
        std::unique_lock guard(m_batchesMutex);
        m_batchesCv.wait(guard, [&]() {
            return m_dispatchingFinished || !m_batches.empty();
        });
        if (m_dispatchingFinished && m_batches.empty()) {
            return false;
        }
        queue = std::move(m_batches.front());
        m_batches.pop();
    }
    interpreter.interpret(std::move(queue), m_cancel);
    return !m_cancel;
}
