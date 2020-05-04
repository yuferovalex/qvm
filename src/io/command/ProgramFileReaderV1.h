#ifndef QVM_PROGRAMFILEREADERV1_H
#define QVM_PROGRAMFILEREADERV1_H

#include <fstream>
#include <functional>
#include <utility>
#include <iterator>

#include <boost/filesystem/path.hpp>

#include "qvm/ProgramFile.h"
#include "qvm/ParameterMetadata.h"

class StringVault;

class ProgramFileReaderV1 {
public:
    typedef std::vector<ProgramFileV1::CommandWithHints>::const_iterator command_iterator;

    ProgramFileReaderV1(const boost::filesystem::path &filePath, StringVault &stringVault);

    const std::string &programDescription() const;

    size_t memorySize() const;

    const std::vector<ParameterMetadata> &inputParamsMeta() const;

    const std::vector<ParameterMetadata> &outputParamsMeta() const;

    command_iterator commandStreamBegin() const;

    command_iterator commandStreamEnd() const;

private:
    void readHeader(std::ifstream &is);

    void readProgramMetadata(std::ifstream &is);

    void readCommands(std::ifstream &is, uint64_t commandsCount);

    static std::vector<ParameterMetadata> readParams(std::ifstream &is, size_t count);

    void readStrings(std::ifstream &is, size_t count);

    static Version parseVersion(char *begin);

    std::vector<ProgramFileV1::CommandWithHints> m_commands;
    std::vector<ParameterMetadata> m_inputParams;
    std::vector<ParameterMetadata> m_outputParams;
    std::string m_programDescription;
    Version m_version{};
    size_t m_memorySize = 0;
    StringVault &m_stringVault;
};

#endif //QVM_PROGRAMFILEREADERV1_H
