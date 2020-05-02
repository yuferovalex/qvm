include_directories(${gtest_SOURCE_DIR}/include)
include_directories(${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include)
include_directories(${gmock_SOURCE_DIR})

add_definitions(-DQVM_TEST_MODE)

file(COPY io/command/product2x2.qvm DESTINATION ${vm_tests_BINARY_DIR}/src/io/command)
file(COPY io/data/input.csv DESTINATION ${vm_tests_BINARY_DIR}/src/io/data)

add_executable(qvm_tests
        test-utils/PseudoAssemblerLanguage.h
        interpreter/interpreter_test.cpp
        interpreter/command_handlers_tests.cpp
        memory/shared_memory_test.cpp
        dispatcher/dispatcher_test.cpp
        io/command/command_file_reader_v1_test.cpp
        io/data/csv_data_reader_test.cpp app/options_test.cpp)

target_link_libraries(qvm_tests gtest gtest_main gmock gmock_main)
target_link_libraries(qvm_tests qvm_lib ${Boost_LIBRARIES})
