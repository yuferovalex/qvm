#ifndef QVM_DISPATCHER_H
#define QVM_DISPATCHER_H

#include <atomic>
#include <queue>
#include <memory>
#include <mutex>
#include <vector>
#include <thread>
#include <condition_variable>

#include "interpreter/Interpreter.h"
#include "dispatcher/DispatcherConfig.h"
#include "utils/CancellationToken.h"

/**
 * Диспетчер задач ВМ.
 */
class Dispatcher {
public:
    typedef Interpreter::CommandQueue Batch;

    /**
     * Конструктор.
     *
     * @param config Конфигурация диспетчера задач.
     */
    explicit Dispatcher(DispatcherConfig &config);

    /**
     * Выполнить программу.
     *
     * @tparam InputIterator Тип итератора для чтения команд, входящих в программу.
     * @param begin          Итератор для чтения команд, входящих в программу.
     * @param end            Итератор, указывающий на конец потока команд, входящих в программу.
     */
    template<typename InputIterator>
    void execute(InputIterator &&begin, InputIterator &&end) {
        auto threads = createThreads();
        createBatches(begin, end);
        waitForAll(std::move(threads));
    }

    bool wasCanceled() const;

    std::string cancelReason() const;

private:
    /**
     * Создать потоки для работы.
     *
     * @return Коллекция из рабочих потоков.
     */
    std::vector<std::thread> createThreads();

    /**
     * Подождать завершения всех потоков.
     * @param threads Потоки, завершение которых необходимо дождаться.
     */
    static void waitForAll(std::vector<std::thread> &&threads);

    /**
     * Создать пакеты заданий для интерпретаторов.
     *
     * @tparam InputIterator Тип итератора для чтения команд, входящих в программу.
     * @param begin          Итератор для чтения команд, входящих в программу.
     * @param end            Итератор, указывающий на конец потока команд, входящих в программу.
     */
    template<typename InputIterator>
    void createBatches(InputIterator &&begin, InputIterator &&end) {
        for (auto batchSize = m_config.batchSize(); begin != end;) {
            // Создаем пакет
            Batch batch{batchSize};
            for (size_t i = 0; i < batchSize && begin != end; i++) {
                batch.push_back(*begin++);
            }

            // Кладем его в очередь пакетов
            {
                std::unique_lock guard(m_batchesMutex);
                m_batches.push(std::move(batch));
                m_batchesCv.notify_one();
            }
        }
        m_dispatchingFinished = true;
        m_batchesCv.notify_all();
    }

    bool handleNextBatch(Interpreter &interpreter);

    std::queue<Batch> m_batches;
    std::mutex m_batchesMutex;
    std::condition_variable m_batchesCv;
    std::atomic_bool m_dispatchingFinished;
    DispatcherConfig &m_config;
    CancellationToken m_cancel;
};

#endif //QVM_DISPATCHER_H
