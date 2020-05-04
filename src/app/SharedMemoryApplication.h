#ifndef QVM_SHAREDMEMORYAPPLICATION_H
#define QVM_SHAREDMEMORYAPPLICATION_H

#include <boost/filesystem/path.hpp>

#include "qvm/ParameterMetadata.h"
#include "utils/Timer.h"

class ProgramFileReaderV1;

class SharedMemoryApplication {
public:
    SharedMemoryApplication(int argc, const char **argv);
    int run();

private:
    static void validateProgramOption(const boost::filesystem::path &programPath);
    static void printProgramInfo(ProgramFileReaderV1 &reader);

    int m_argc;
    const char **m_argv;
    Timer m_executionTimer;
};


#endif //QVM_SHAREDMEMORYAPPLICATION_H
