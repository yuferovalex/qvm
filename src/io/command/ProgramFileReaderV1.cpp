#include <cstdint>
#include <cstring>

#include "qvm/ProgramFile.h"
#include "ProgramFileReaderV1.h"
#include "memory/StringVault.h"

using namespace ProgramFileV1;

namespace {
    template<typename T>
    T read(std::istream &is) {
        union {
            T asResult{};
            char asByteArray[sizeof(T)];
        } value;
        auto src = std::istreambuf_iterator(is);
        auto *dest = value.asByteArray;
        for (int i = 0; i < sizeof(T); ++i) {
            *dest++ = *src++;
        }
        return value.asResult;
    }
}

ProgramFileReaderV1::ProgramFileReaderV1(const boost::filesystem::path &filePath, StringVault &stringVault)
    : m_stringVault(stringVault)
{
    std::ifstream is(filePath.string(), std::ios::binary);
    readHeader(is);
    readProgramMetadata(is);
}

void ProgramFileReaderV1::readHeader(std::ifstream &is) {
    auto header = read<FileHeader>(is);
    if (header.magic != MAGIC_NUMBER) {
        throw std::runtime_error("given command is not a QVM program, or has wrong magic number");
    }
    m_version = parseVersion(header.version);
    if (m_version.major != 1) {
        throw std::runtime_error("unknown command version. can handle only files with major version 1");
    }
}

void ProgramFileReaderV1::readProgramMetadata(std::ifstream &is) {
    auto metadata = read<ProgramMetadataMarshaled>(is);
    m_programDescription = metadata.description;
    m_memorySize = metadata.memorySize;
    m_inputParams = readParams(is, metadata.inputParametersCount);
    m_outputParams = readParams(is, metadata.outputParametersCount);
    readStrings(is, metadata.stringConstantsCount);
    readCommands(is, metadata.commandsCount);
}

std::vector<ParameterMetadata> ProgramFileReaderV1::readParams(std::ifstream &is, size_t count) {
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

static std::string readString(std::ifstream &is, size_t &readBytes) {
    std::string result;
    for (char ch = 0; is.get(ch) && ch != '\0';) {
        result += ch;
    }
    readBytes += result.size() + 1;
    return result;
}

void ProgramFileReaderV1::readStrings(std::ifstream &is, size_t count) {
    size_t readBytes = 0;
    for (size_t i = 0; i < count; ++i) {
        auto string = readString(is, readBytes);
        m_stringVault.addString(string);
    }

    // Выравниевание
    if (readBytes != 0) {
        std::istream_iterator<char> begin(is);
        std::advance(begin, 16 - (readBytes % 16) - 1);
    }
}

Version ProgramFileReaderV1::parseVersion(char *begin) {
    auto *end = begin + strlen(begin);
    auto *firstPoint = std::find(begin, end, '.');
    auto *secondPoint = end;
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

void ProgramFileReaderV1::readCommands(std::ifstream &is, uint64_t commandsCount) {
    m_commands.reserve(commandsCount);
    for (uint64_t i = 0; i < commandsCount; ++i) {
        m_commands.emplace_back(read<CommandWithHints>(is));
    }
}

const std::string &ProgramFileReaderV1::programDescription() const {
    return m_programDescription;
}

size_t ProgramFileReaderV1::memorySize() const {
    return m_memorySize;
}

ProgramFileReaderV1::command_iterator ProgramFileReaderV1::commandStreamBegin() const {
    return m_commands.cbegin();
}

ProgramFileReaderV1::command_iterator ProgramFileReaderV1::commandStreamEnd() const {
    return m_commands.cend();
}

const std::vector<ParameterMetadata> &ProgramFileReaderV1::inputParamsMeta() const {
    return m_inputParams;
}

const std::vector<ParameterMetadata> &ProgramFileReaderV1::outputParamsMeta() const {
    return m_outputParams;
}
