cmake_minimum_required(VERSION 3.15)
project(vm)

set(CMAKE_CXX_STANDARD 17)

set(BOOST_ROOT $ENV{BOOST_LIB_ROOT})

find_package(Boost 1.72.0 REQUIRED)

include_directories(src ${Boost_INCLUDE_DIRS})
add_subdirectory(src)
add_subdirectory(test)

add_executable(vm src/main.cpp)
target_link_libraries(vm vm_lib)


