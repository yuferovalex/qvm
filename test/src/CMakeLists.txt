include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

add_definitions(-DVM_TEST_MODE)

add_executable(vm_tests
        interpreter/interpreter_test.cpp
        memory/shared_memory_test.cpp
        interpreter/command_handlers_tests.cpp
        dispatcher/dispatcher_test.cpp)

target_link_libraries(vm_tests gtest gtest_main gmock gmock_main)
target_link_libraries(vm_tests vm_lib)
