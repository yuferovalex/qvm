#ifndef VM_APPLICATION_H
#define VM_APPLICATION_H

#include <boost/filesystem/path.hpp>

#include "qvm/ParameterMetadata.h"

class CommandFileReaderV1;

class SharedMemoryApplication {
public:
    SharedMemoryApplication(int argc, const char **argv);
    int run();

private:
    static void validateProgramOption(const boost::filesystem::path &programPath);
    static void printProgramInfo(CommandFileReaderV1 &reader);

    int m_argc;
    const char **m_argv;
};


#endif //VM_APPLICATION_H
