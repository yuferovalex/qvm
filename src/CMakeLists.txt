set(HEADER_FILES
        memory/Memory.h
        memory/SharedMemory.h
        interpreter/Command.h
        interpreter/InterpreterImpl.h
        interpreter/CommandHandlersImpl.h
        dispatcher/Dispatcher.h
        dispatcher/DispatcherConfigImpl.h
        interpreter/Interpreter.h
        dispatcher/DispatcherConfig.h ../test/src/interpreter/PseudoAsemblerLanguage.h)

set(SOURCE_FILES
        memory/SharedMemory.cpp
        interpreter/CommandHandlersImpl.cpp
        interpreter/InterpreterImpl.cpp dispatcher/Dispatcher.cpp dispatcher/DispatcherConfigImpl.cpp interpreter/CommandHandler.h)

add_library(vm_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
