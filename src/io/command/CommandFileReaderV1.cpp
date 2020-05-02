#include <cstdint>
#include <cstring>

#include "qvm/Command.h"
#include "CommandFileReaderV1.h"

using namespace ProgramFileV1;

namespace {
    template<typename T>
    T read(std::istream &is) {
        T value{};
        auto src = std::istreambuf_iterator(is);
        auto dest = reinterpret_cast<std::istream::char_type *>(&value);
        for (int i = 0; i < sizeof(T); ++i) {
            *dest++ = *src++;
        }
        return value;
    }
}

CommandFileReaderV1::CommandFileReaderV1(const std::string &filePath) {
    std::ifstream is(filePath, std::ios::binary);
    readHeader(is);
    readProgramMetadata(is);
}

void CommandFileReaderV1::readHeader(std::ifstream &is) {
    auto header = read<FileHeader>(is);
    if (header.magic != MAGIC_NUMBER) {
        throw std::runtime_error("given command is not a QVM program, or has wrong magic number");
    }
    m_version = parseVersion(header.version);
    if (m_version.major != 1) {
        throw std::runtime_error("unknown command version. can handle only files with major version 1");
    }
}

void CommandFileReaderV1::readProgramMetadata(std::ifstream &is) {
    auto metadata = read<ProgramMetadataMarshaled>(is);
    m_programDescription = metadata.description;
    m_memorySize = metadata.memorySize;
    m_inputParams = readParams(is, metadata.inputParametersCount);
    m_outputParams = readParams(is, metadata.outputParametersCount);
    readCommands(is, metadata.commandsCount);
}

std::vector<ParameterMetadata> CommandFileReaderV1::readParams(std::ifstream &is, size_t count) {
    std::vector<ParameterMetadata> params;
    params.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        auto marshaled = read<ParameterMetadataMarshaled>(is);

        ParameterMetadata meta;
        meta.name = marshaled.name;
        meta.description = marshaled.description;
        meta.address = marshaled.memoryAddress;
        meta.rows = marshaled.rows;
        meta.columns = marshaled.columns;
        params.emplace_back(meta);
    }
    return params;
}

Version CommandFileReaderV1::parseVersion(char *begin) {
    auto end = begin + strlen(begin);
    auto firstPoint = std::find(begin, end, '.');
    auto secondPoint = end;
    if (firstPoint != end) {
        secondPoint = std::find(firstPoint + 1, end, '.');
    }
    if (firstPoint == end || secondPoint == end) {
        throw std::runtime_error("given command has wrong format of version number");
    }

    Version version;
    size_t pos = 0;
    version.major = std::stoi(begin, &pos);
    begin += pos + 1;
    version.minor = std::stoi(begin, &pos);
    begin += pos + 1;
    version.bugFix = std::stoi(begin);
    return version;
}

void CommandFileReaderV1::readCommands(std::ifstream &is, uint64_t commandsCount) {
    m_commands.reserve(commandsCount);
    for (uint64_t i = 0; i < commandsCount; ++i) {
        m_commands.emplace_back(read<CommandWithHints>(is));
    }
}

std::string CommandFileReaderV1::programDescription() const {
    return m_programDescription;
}

size_t CommandFileReaderV1::memorySize() const {
    return m_memorySize;
}

CommandFileReaderV1::command_iterator CommandFileReaderV1::commandStreamBegin() const {
    return m_commands.cbegin();
}

CommandFileReaderV1::command_iterator CommandFileReaderV1::commandStreamEnd() const {
    return m_commands.cend();
}

const std::vector<ParameterMetadata> &CommandFileReaderV1::inputParamsMeta() const {
    return m_inputParams;
}

const std::vector<ParameterMetadata> &CommandFileReaderV1::outputParamsMeta() const {
    return m_outputParams;
}
