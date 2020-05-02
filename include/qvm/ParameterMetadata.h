#ifndef QVM_PARAMETERMETADATA_H
#define QVM_PARAMETERMETADATA_H

#include <cstdint>
#include <string>

#include "qvm/Command.h"

/**
 * Метаданные входных и выходных параметров.
 */
class ParameterMetadata {
public:
    /**
     * Наименование параметра.
     */
    std::string name;

    /**
     * Описание параметра.
     */
    std::string description;

    /**
     * Адрес данных в памяти ВМ.
     */
    Command::Reference address;

    /**
     * Кол-во строк.
     */
    uint64_t rows;

    /**
     * Кол-во колонок.
     */
    uint64_t columns;

    /**
     * Объем данных.
     */
    uint64_t size() const {
        return uint64_t(rows) * uint64_t(columns);
    }
};

#endif //QVM_PARAMETERMETADATA_H
