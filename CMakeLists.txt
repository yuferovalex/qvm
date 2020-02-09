cmake_minimum_required(VERSION 3.15)
project(vm)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
add_subdirectory(src)
add_subdirectory(test)

add_executable(vm src/main.cpp)
target_link_libraries(vm vm_lib)
