#include <stdexcept>
#include "SharedMemory.h"

SharedMemory::SharedMemory(size_t allocated)
    : m_vault(allocated)
{}

std::optional<Command::Value> SharedMemory::value(Command::Reference ref) const {
#ifndef NDEBUG
    if (ref == 0) {
        throw std::logic_error("attempt to check null reference");
    }
    if (ref > m_vault.size()) {
        throw std::logic_error("attempt to check outbound reference");
    }
#endif
    auto &value = m_vault[ref - 1];
    if (!value.set) {
        return std::nullopt;
    }
    return std::optional(value.value);
}

void SharedMemory::setValue(Command::Reference ref, Command::Value value) {
#ifndef NDEBUG
    if (ref == 0) {
        throw std::logic_error("attempt to check null reference");
    }
    if (ref > m_vault.size()) {
        throw std::logic_error("attempt to check outbound reference");
    }
#endif
    auto &cell = m_vault[ref - 1];
    cell.value = value;
    cell.set = true;
}
