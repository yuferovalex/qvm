#ifndef VM_COMMANDFILEREADERV1_H
#define VM_COMMANDFILEREADERV1_H

#include <fstream>
#include <functional>
#include <utility>
#include <iterator>

#include "qvm/ProgramFile.h"
#include "qvm/ParameterMetadata.h"

class CommandFileReaderV1 {
public:
    typedef std::vector<CommandWithHints>::const_iterator command_iterator;

    explicit CommandFileReaderV1(const std::string &filePath);

    std::string programDescription() const;

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

    static Version parseVersion(char *begin);

    std::vector<CommandWithHints> m_commands;
    std::vector<ParameterMetadata> m_inputParams;
    std::vector<ParameterMetadata> m_outputParams;
    std::string m_programDescription;
    Version m_version{};
    size_t m_memorySize = 0;
};

#endif //VM_COMMANDFILEREADERV1_H
