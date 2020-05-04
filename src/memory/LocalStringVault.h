#ifndef QVM_LOCALSTRINGVAULT_H
#define QVM_LOCALSTRINGVAULT_H

#include <mutex>
#include <vector>

#include "memory/StringVault.h"

/**
 * Локальное хранилище строк.
 */
class LocalStringVault : public StringVault {
public:
    const std::string &string(uint32_t id) const override;
    uint32_t addString(const std::string &string) override;

private:
    mutable std::mutex m_mutex;
    std::vector<std::string> m_strings;
};


#endif //QVM_LOCALSTRINGVAULT_H
