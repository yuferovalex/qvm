#ifndef VM_PARAMMETA_H
#define VM_PARAMMETA_H

#include <cstdint>
#include <string>

#include "interpreter/Command.h"

/**
 * Метаданные входных и выходных параметров.
 */
class ParamMeta {
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

#endif //VM_PARAMMETA_H
