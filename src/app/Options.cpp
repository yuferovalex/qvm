#include "Options.h"

#include <iostream>
#include <thread>
#include <boost/filesystem/operations.hpp>
#include <utils/utils.h>

#include "dispatcher/DispatcherConfigImpl.h"

Options::Options(int argn, const char **argv)
    : m_argn(argn), m_argv(argv)
{
    fillOptionsDescription();
    parse();
}

void Options::fillOptionsDescription() {
    boost::program_options::options_description qvmParams("QVM options");
    qvmParams.add_options()
            (
                    "help,h",
                    boost::program_options::bool_switch(&m_help)->default_value(false),
                    "show this help"
            )
            (
                    "version,v",
                    boost::program_options::bool_switch(&m_version)->default_value(false),
                    "show version of QVM"
            )
            (
                    "program,p",
                    boost::program_options::value(&m_program)->default_value("program.qvm"),
                    "path of program to execute (this is a required parameter)"
            )
            (
                    "info",
                    boost::program_options::bool_switch(&m_showProgramInfo)->default_value(false),
                    "show program description"
            )
            (
                    "threads,t",
                    boost::program_options::value(&m_threadCount)->default_value(DispatcherConfigImpl::DEFAULT_THREAD_COUNT),
                    "setup working threads count (default value is equals to"
                    " \"number of hardware thread contexts - 1\")"
            )
            (
                    "batch,b",
                    boost::program_options::value(&m_batchSize)->default_value(DispatcherConfigImpl::DEFAULT_BATCH_SIZE),
                    "setup work batch size"
            )
            (
                    "timeit",
                    boost::program_options::bool_switch(&m_trackExecutionTime)->default_value(false),
                    "track execution time"
            )
            ;
    m_optionsDescription.add(qvmParams);
}

void Options::parse() {
    auto result = boost::program_options::command_line_parser(m_argn, m_argv)
            .options(m_optionsDescription)
            .allow_unregistered()
            .run();
    boost::program_options::store(result, m_variablesMap);
    boost::program_options::notify(m_variablesMap);
}

void Options::showOptionsDescription() {
    std::cout << m_optionsDescription;
}

boost::filesystem::path Options::param(const std::string &name, bool requiredExists) const {
    auto paramName = "param:" + name;
    if (m_variablesMap[paramName].empty()) {
        throw std::runtime_error("data file for parameter \"" + paramName + "\" not specified");
    }
    const auto &path = m_variablesMap[paramName].as<boost::filesystem::path>();
    if (requiredExists) {
        if (!boost::filesystem::exists(path)) {
            throw std::runtime_error("given path in \"" + paramName + "\" option is not exists");
        }
        if (!boost::filesystem::is_regular_file(path)) {
            throw std::runtime_error("given path in \"" + paramName + "\" option is not a file");
        }
    }
    return path;
}

void Options::parseParams(const std::vector<ParameterMetadata> &input, const std::vector<ParameterMetadata> &output) {
    m_optionsDescription
            .add(buildParamOptions(input, "Input params"))
            .add(buildParamOptions(output, "Output params"));
    parse();
}
