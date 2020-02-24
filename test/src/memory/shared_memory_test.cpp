#include <gtest/gtest.h>

#include "memory/SharedMemory.h"

/**
 * Тест проверяет, что реализация общей памяти ВМ возвращает пустой объект std::optional, если значение
 * не устанавливалось к моменту запроса.
 */
TEST(SharedMemoryTest, value_method_should_return_empty_optional_until_value_is_set) {
    SharedMemory memory(1);

    EXPECT_FALSE(memory.value(1));
}

/**
 * Тест проверяет, что реализация общей памяти ВМ возвращает заполненный объект std::optional, если значение
 * было установлено ранее.
 */
TEST(SharedMemoryTest, value_method_should_return_filled_optional_if_value_is_set) {
    SharedMemory memory(1);

    memory.setValue(1, (Command::Value) { 1.0 });

    EXPECT_TRUE(memory.value(1));
    EXPECT_DOUBLE_EQ(memory.value(1).value().number, 1.0);
}
