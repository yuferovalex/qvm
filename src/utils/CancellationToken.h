#ifndef QVM_CANCELLATIONTOKEN_H
#define QVM_CANCELLATIONTOKEN_H

#include <atomic>
#include <mutex>

class CancellationToken {
public:
    explicit operator bool() const { return m_cancelled; }

    auto reason() const {
        std::unique_lock lock(m_mutex);
        return m_reason;
    }

    void cancel(const std::string &reason) {
        if (!m_cancelled) {
            std::unique_lock lock(m_mutex);
            if (!m_cancelled) {
                m_reason = reason;
                m_cancelled = true;
            }
        }
    }

private:
    mutable std::mutex m_mutex;
    std::atomic<bool> m_cancelled = false;
    std::string m_reason;
};

#endif //QVM_CANCELLATIONTOKEN_H
