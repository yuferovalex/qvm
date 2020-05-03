include_directories(${gtest_SOURCE_DIR}/include)
include_directories(${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include)
include_directories(${gmock_SOURCE_DIR})

add_definitions(-DQVM_TEST_MODE)

add_executable(qvm_tests
        test-utils/PseudoAssemblerLanguage.h
        interpreter/interpreter_test.cpp
        interpreter/command_handlers_tests.cpp
        memory/shared_memory_test.cpp
        dispatcher/dispatcher_test.cpp
        io/command/program_file_reader_v1_test.cpp
        io/data/csv_data_reader_test.cpp app/options_test.cpp app/app_test.cpp)

target_link_libraries(qvm_tests gtest gtest_main gmock gmock_main)
target_link_libraries(qvm_tests qvm_lib ${Boost_LIBRARIES})

add_custom_command(TARGET qvm_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/test/resources ${qvm_tests_BINARY_DIR}/src/resources)
