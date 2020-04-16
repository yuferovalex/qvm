set(HEADER_FILES
        memory/Memory.h
        memory/SharedMemory.h
        interpreter/Command.h
        interpreter/InterpreterImpl.h
        interpreter/CommandHandlersImpl.h
        interpreter/Interpreter.h
        interpreter/CommandHandler.h
        dispatcher/Dispatcher.h
        dispatcher/DispatcherConfigImpl.h
        dispatcher/DispatcherConfig.h
        io/ParamMeta.h
        io/command/CommandFileReaderV1.h
        )

set(SOURCE_FILES
        memory/SharedMemory.cpp
        interpreter/CommandHandlersImpl.cpp
        interpreter/InterpreterImpl.cpp
        dispatcher/Dispatcher.cpp
        dispatcher/DispatcherConfigImpl.cpp
        io/command/CommandFileReaderV1.cpp
        )

add_library(vm_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
