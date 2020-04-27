#ifndef QVM_RESULT_IS_H
#define QVM_RESULT_IS_H

#include <variant>

typedef std::variant<bool, double> Result;

/**
 * Типизированный компаратор результата вычисления команды.
 */
MATCHER_P(ResultIs, expectedResult, "result is") {
    struct Comparator {
        Command::Value actual;

        bool operator()(bool expected) {
            return actual.boolean == expected;
        }

        bool operator()(double expected) {
            return actual.number == expected;
        }
    };

    return std::visit(Comparator { arg }, Result (expectedResult));
}

#endif //QVM_RESULT_IS_H
