set(HEADER_FILES utils.h)

set(SOURCE_FILES utils.cpp)

add_library(vm_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
