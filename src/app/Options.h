#ifndef VM_VMOPTIONS_H
#define VM_VMOPTIONS_H

#include <boost/filesystem/path.hpp>
#include <boost/program_options.hpp>

#include "io/data/CsvDataReader.h"

class Options : public ParametersPathProvider {
public:
    Options(int argn, const char **argv);

    bool isShowVersion() const {
        return m_version;
    }

    bool isShowHelp() const {
        return m_help;
    }

    const boost::filesystem::path &programPath() const {
        return m_program;
    }

    uint32_t threadCount() const {
        return m_threadCount;
    }

    size_t batchSize() const {
        return m_batchSize;
    }

    bool isShowProgramInfo() const {
        return m_showProgramInfo;
    }

    boost::filesystem::path param(const std::string &name, bool requiredExists) const override;

    void showOptionsDescription();

    void parseParams(const std::vector<ParameterMetadata> &input, const std::vector<ParameterMetadata> &output);

private:
    void fillOptionsDescription();
    void parse();

    boost::program_options::options_description m_optionsDescription;
    boost::program_options::variables_map m_variablesMap;
    boost::filesystem::path m_program;
    const char **m_argv;
    int m_argn;
    size_t m_batchSize;
    uint32_t m_threadCount;
    bool m_help;
    bool m_version;
    bool m_showProgramInfo;
};

#endif //VM_VMOPTIONS_H
