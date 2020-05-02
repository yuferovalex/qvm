set(HEADER_FILES
        memory/Memory.h
        memory/SharedMemory.h
        ../include/qvm/Command.h
        interpreter/InterpreterImpl.h
        interpreter/CommandHandlersImpl.h
        interpreter/Interpreter.h
        interpreter/CommandHandler.h
        dispatcher/Dispatcher.h
        dispatcher/DispatcherConfigImpl.h
        dispatcher/DispatcherConfig.h
        ../include/qvm/ParameterMetadata.h
        io/command/CommandFileReaderV1.h
        app/Options.h
        app/SharedMemoryApplication.h
        utils/utils.h)

set(SOURCE_FILES
        memory/SharedMemory.cpp
        interpreter/CommandHandlersImpl.cpp
        interpreter/InterpreterImpl.cpp
        dispatcher/Dispatcher.cpp
        dispatcher/DispatcherConfigImpl.cpp
        io/command/CommandFileReaderV1.cpp
        app/Options.cpp
        app/SharedMemoryApplication.cpp
        io/data/CsvDataReader.cpp io/data/CsvDataReader.h utils/utils.cpp)

add_library(qvm_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(qvm_lib ${Boost_LIBRARIES} Threads::Threads)
