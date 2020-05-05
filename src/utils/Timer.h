#ifndef QVM_TIMER_H
#define QVM_TIMER_H

#include <string>
#include <sstream>
#include <mutex>
#include <chrono>

class Timer {
public:
    typedef std::chrono::high_resolution_clock clock_type;

    explicit Timer(const std::string &threadName);

    void setExecuting(bool executing);

    void start(const std::string &newBlockName);

    void finish();

    std::string results() const;

private:
    static std::stringstream m_os;
    static std::mutex m_osMutex;
    std::string m_threadName;
    std::string m_prevBlockName;
    clock_type::time_point m_prevBlockBegin = clock_type::time_point::min();
    bool m_executing = false;
};

#endif //QVM_TIMER_H
