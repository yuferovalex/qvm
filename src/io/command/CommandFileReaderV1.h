#ifndef VM_COMMANDFILEREADERV1_H
#define VM_COMMANDFILEREADERV1_H

#include <fstream>
#include <functional>
#include <utility>
#include <iterator>

#include "io/ParamMeta.h"

class CommandFileReaderV1 {
public:
    /**
     * Команда с подсказками для диспетчера задач ВМ.
     */
    struct alignas(64) CommandWithHints : Command {
    };

    static_assert(sizeof(CommandWithHints) == 64, "The size of struct CommandWithHints must be 64 bytes");

    typedef std::vector<CommandWithHints>::const_iterator command_iterator;

    explicit CommandFileReaderV1(const std::string &filePath);

    std::string programDescription() const;

    size_t memorySize() const;

    const std::vector<ParamMeta> &inputParamsMeta() const;

    const std::vector<ParamMeta> &outputParamsMeta() const;

    command_iterator commandStreamBegin() const;

    command_iterator commandStreamEnd() const;

private:
    /**
     * Версия протокола файла программы.
     */
    struct Version {
        unsigned major = 0;
        unsigned minor = 0;
        unsigned bugFix = 0;
    };

    void readHeader(std::ifstream &is);

    void readProgramMetadata(std::ifstream &is);

    void readCommands(std::ifstream &is, uint64_t commandsCount);

    static std::vector<ParamMeta> readParams(std::ifstream &is, size_t count);

    static Version parseVersion(char *begin);

    std::vector<CommandWithHints> m_commands;
    std::vector<ParamMeta> m_inputParams;
    std::vector<ParamMeta> m_outputParams;
    std::string m_programDescription;
    Version m_version{};
    size_t m_memorySize = 0;
};

#endif //VM_COMMANDFILEREADERV1_H
