#include "SharedMemoryApplication.h"

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <memory>

#include <boost/filesystem/operations.hpp>

#include "app/Config.h"
#include "app/Options.h"
#include "dispatcher/DispatcherConfigImpl.h"
#include "dispatcher/Dispatcher.h"
#include "interpreter/CommandHandlersImpl.h"
#include "io/command/ProgramFileReaderV1.h"
#include "io/data/CsvDataReader.h"
#include "memory/SharedMemory.h"
#include "memory/LocalStringVault.h"
#include "utils/utils.h"
#include "utils/Timer.h"

SharedMemoryApplication::SharedMemoryApplication(int argc, const char **argv)
    : m_argc(argc), m_argv(argv)
{
    registerCommandHandlers();
}

int SharedMemoryApplication::run() {
    try {
        Options options(m_argc, m_argv);

        if (options.isShowVersion()) {
            std::cout << PROJECT_NAME << " " << PROJECT_VER << std::endl;
            return EXIT_SUCCESS;
        }

        if (options.isShowHelp() && options.programPath().empty()) {

            return EXIT_SUCCESS;
        }

        auto programPath = options.programPath();
        validateProgramOption(programPath);

        LocalStringVault stringVault;
        Timer executionTimer("MAIN THREAD");
        executionTimer.setExecuting(options.trackExecutionTime());
        executionTimer.start("READING PROGRAM FILE");
        ProgramFileReaderV1 programReader(programPath, stringVault);
        executionTimer.finish();

        options.parseParams(programReader.inputParamsMeta(), programReader.outputParamsMeta());

        if (options.isShowProgramInfo()) {
            printProgramInfo(programReader);
            return EXIT_SUCCESS;
        }

        if (options.isShowHelp()) {
            options.showOptionsDescription();
            return EXIT_SUCCESS;
        }
        
        SharedMemory memory(programReader.memorySize());
        CsvDataReader dataReader(memory, options);
        DispatcherConfigImpl dispatcherConfig(memory, stringVault,
                options.batchSize(),
                options.threadCount());
        Dispatcher dispatcher(dispatcherConfig);

        executionTimer.start("READING INPUT DATA FROM FILES");
        dataReader.readInput(programReader.inputParamsMeta());

        executionTimer.start("EXECUTING PROGRAM");
        dispatcher.execute(programReader.commandStreamBegin(), programReader.commandStreamEnd());

        if (dispatcher.wasCanceled()) {
            std::cout << executionTimer.results() << dispatcher.cancelReason() << std::endl;
            return EXIT_SUCCESS;
        }

        executionTimer.start("WRITING OUTPUT DATA TO FILES");
        dataReader.writeOutput(programReader.outputParamsMeta());
        executionTimer.finish();

        std::cout << executionTimer.results();

        return EXIT_SUCCESS;
    }
    catch (std::exception &e) {
        std::cerr << "Error occurred: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown fatal error occurred" << std::endl;
    }
    return EXIT_FAILURE;
}

void SharedMemoryApplication::validateProgramOption(const boost::filesystem::path &programPath) {
    if (programPath.empty()) {
        throw std::runtime_error("\"program\" is required");
    }

    if (!boost::filesystem::exists(programPath)) {
        throw std::runtime_error("given path in \"program\" option is not exists");
    }

    if (!boost::filesystem::is_regular_file(programPath)) {
        throw std::runtime_error("given path in \"program\" option is not a file");
    }
}

void SharedMemoryApplication::printProgramInfo(ProgramFileReaderV1 &reader) {
    std::cout << "Program description:" << std::endl
              << reader.programDescription() << std::endl << std::endl
              << buildParamOptions(reader.inputParamsMeta(), "Input parameters") << std::endl
              << buildParamOptions(reader.outputParamsMeta(), "Output parameters") << std::endl;
}
