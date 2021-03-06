#include <gtest/gtest.h>

#include "interpreter/CommandHandlersImpl.h"
#include "dispatcher/DispatcherConfigImpl.h"
#include "dispatcher/Dispatcher.h"
#include "memory/SharedMemory.h"
#include "memory/LocalStringVault.h"

#include "test-utils/PseudoAssemblerLanguage.h"

TEST(DispatcherTest, non_conditional_terms) {
    registerCommandHandlers();

    SharedMemory sharedMemory { 3 };
    LocalStringVault stringVault;
    DispatcherConfigImpl dispatcherConfig(sharedMemory, stringVault, 2, 3);
    Dispatcher dispatcher (dispatcherConfig);

    // Определитель 2-го порядка
    // | 1 2 |
    // | 3 4 |

    auto program = QVM_ASM_BEGIN(std::vector<Command>())
        CMD_MUL(VAL(1.0), VAL(4.0), 1);
        CMD_MUL(VAL(2.0), VAL(3.0), 2);
        CMD_SUB(REF(1), REF(2), 3);
    QVM_ASM_END;

    dispatcher.execute(program.begin(), program.end());

    EXPECT_DOUBLE_EQ(-2.0, sharedMemory.value(3)->number);
}

TEST(DispatcherTest, conditional_terms) {
    registerCommandHandlers();

    SharedMemory sharedMemory { 3 };
    LocalStringVault stringVault;
    DispatcherConfigImpl dispatcherConfig(sharedMemory, stringVault, 2, 3);
    Dispatcher dispatcher (dispatcherConfig);

    auto program = QVM_ASM_BEGIN(std::vector<Command>())
        CMD_MUL(VAL(1.0), VAL(4.0), 1);
        CMD_MUL(VAL(2.0), VAL(3.0), 2);
        CMD_SUB(REF(1), REF(2), 3);
    QVM_ASM_END;

    dispatcher.execute(program.begin(), program.end());

    EXPECT_DOUBLE_EQ(-2.0, sharedMemory.value(3)->number);
}
