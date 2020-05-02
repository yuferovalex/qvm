cmake_minimum_required(VERSION 3.15)
project(qvm VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost 1.65.0 COMPONENTS program_options filesystem REQUIRED)
find_package(Threads REQUIRED)

include_directories(include)
include_directories(src)
include_directories(${Boost_INCLUDE_DIRS})

configure_file(src/app/Config.h.template ${PROJECT_BINARY_DIR}/src/app/Config.h)
include_directories(${PROJECT_BINARY_DIR}/src)

add_subdirectory(src)
add_subdirectory(test)

add_executable(qvm src/main.cpp)
target_link_libraries(qvm qvm_lib)
