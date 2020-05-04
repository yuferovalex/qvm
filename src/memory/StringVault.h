#ifndef QVM_STRINGVAULT_H
#define QVM_STRINGVAULT_H

#include <cstdint>
#include <string>

/**
 * Хранилище константных строк.
 */
class StringVault {
public:
    virtual ~StringVault() = default;

    /**
     * Получить строчку по её идентификатору.
     *
     * @param id Идентификатор строки.
     * @return Возвращает строчку.
     * @throws out_of_range Исключение выбрасывается при отсуствии
     *         строки с переданным идентификатором.
     */
    virtual const std::string &string(uint32_t id) const = 0;

    /**
     * Добавить строку на хранение.
     *
     * @param string Строка, подлежащая хранению.
     * @return Возвращает идентификатор строки.
     */
    virtual uint32_t addString(const std::string &string) = 0;
};

#endif //QVM_STRINGVAULT_H
