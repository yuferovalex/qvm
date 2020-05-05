#include "Timer.h"

#include <iostream>

std::stringstream Timer::m_os{};
std::mutex Timer::m_osMutex{};

Timer::Timer(const std::string &threadName)
    : m_threadName(threadName)
{}

void Timer::start(const std::string &newBlockName) {
    if (!m_executing) {
        return;
    }
    finish();
    m_prevBlockBegin = clock_type::now();
    m_prevBlockName = newBlockName;
}

void Timer::finish() {
    if (!m_executing) {
        return;
    }
    if (m_prevBlockBegin != clock_type::time_point::min()) {
        using std::chrono::duration_cast;
        typedef std::chrono::duration<double, std::milli> milliseconds;
        auto duration = duration_cast<milliseconds>(clock_type::now() - m_prevBlockBegin);

        std::lock_guard guard(m_osMutex);
        m_os << m_threadName << ": " << m_prevBlockName << " ELAPSED TIME: "
             << duration.count() << " ms\n";
    }
    m_prevBlockBegin = clock_type::time_point::min();
}

void Timer::setExecuting(bool executing) {
    m_executing = executing;
}

std::string Timer::results() const {
    std::lock_guard guard(m_osMutex);
    return m_os.str();
}
