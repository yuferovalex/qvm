#include "LocalStringVault.h"

const std::string &LocalStringVault::string(uint32_t id) const {
    std::unique_lock lock(m_mutex);
    return m_strings.at(id);
}

uint32_t LocalStringVault::addString(const std::string &string) {
    std::unique_lock lock(m_mutex);
    m_strings.push_back(string);
    return m_strings.size() - 1;
}
