include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

add_definitions(-DVM_TEST_MODE)

file(COPY io/command/product2x2.qvm DESTINATION ${vm_tests_BINARY_DIR}/src/io/command)

add_executable(vm_tests
        interpreter/PseudoAssemblerLanguage.h
        interpreter/interpreter_test.cpp
        interpreter/command_handlers_tests.cpp
        memory/shared_memory_test.cpp
        dispatcher/dispatcher_test.cpp
        io/command/command_file_reader_v1_test.cpp
        )

target_link_libraries(vm_tests gtest gtest_main gmock gmock_main)
target_link_libraries(vm_tests vm_lib)
