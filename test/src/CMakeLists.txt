include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(vm_tests utils_test.cpp)

target_link_libraries(vm_tests gtest gtest_main)
target_link_libraries(vm_tests vm_lib)
