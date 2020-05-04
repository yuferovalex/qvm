#ifndef QVM_TIMER_H
#define QVM_TIMER_H

#include <chrono>

class Timer {
public:
    typedef std::chrono::high_resolution_clock clock_type;

    clock_type::duration duration() const { return m_duration; }

    void start() { m_begin = clock_type::now(); }

    void stop() {
        m_end = clock_type::now();
        m_duration = m_end - m_begin;
    }

private:
    clock_type::time_point m_begin;
    clock_type::time_point m_end;
    clock_type::duration m_duration;
};

#endif //QVM_TIMER_H
