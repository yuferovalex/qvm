#include <cstdint>
#include <cstring>

#include "interpreter/Command.h"
#include "CommandFileReaderV1.h"

namespace {

    /**
     * Магическое число программ для QVM.
     * <p/>
     * При открытии программы в текстовом редакторе в кодировке ASCII будет выглядеть как строчка "QVMPROG",
     * ограниченная нулем.
     */
    const uint64_t MAGIC_NUMBER = 0x00474F52504D5651LL;

    /**
     * Общий заголовок для всех версий протоколов.
     */
    struct FileHeader {
        /**
         * Магическое число.
         */
        uint64_t magic;

        /**
         * Версия файла в формате <major>.<minor>.<bug_fix>
         * <p/>
         * Если в заголовок нужно добавить новое значение, то меняется число minor.
         * Если происходит какое-то незначительное изменение протокола, которое не должно повлечь изменений в
         * коде чтения, то меняется число bug_fix.
         * Если требуется изменить размер данной структуры или иной из этого пространства имен, то меняется
         * основной номер версии данного протокола.
         * Номера записываются с помощью ASCII символов, чтобы можно было в текстовом редакторе определить номер
         * версии не прибегая к помощи дополнительных инструментов.
         */
        char version[8];
    };

    static_assert(sizeof(FileHeader) == 16, "The size of struct FileHeader must be 16 bytes");

    /**
     * Заголовок файла с программой.
     */
    struct alignas(512) ProgramMetadataMarshaled {
        /**
         * Текстовое описание программы.
         */
        char description[256];

        /**
         * Размер памяти, выделяемой для работы программы.
         */
        uint64_t memorySize;

        /**
         * Кол-во входных параметров.
         */
        uint32_t inputParametersCount;

        /**
         * Кол-во входных параметров.
         */
        uint32_t outputParametersCount;

        /**
         * Кол-во команд в программе.
         */
        uint64_t commandsCount;
    };

    static_assert(sizeof(ProgramMetadataMarshaled) == 512,
                  "The size of struct ProgramMetadataMarshaled must be 512 bytes");

    /**
     * Мета-данные параметров.
     */
    struct alignas(128) ParameterMetadataMarshaled {
        /**
         * Наименование внутри алгоритма.
         */
        char name[16];

        /**
         * Описание для пользователя.
         */
        char description[64];

        /**
         * Начальный адрес в памяти.
         */
        Command::Reference memoryAddress;

        /**
         * Кол-во строк в матрице.
         */
        uint32_t rows;

        /**
         * Кол-во столбцов в матрице.
         */
        uint32_t columns;
    };

    static_assert(sizeof(ParameterMetadataMarshaled) == 128,
                  "The size of struct ParameterMetadataMarshaled must be 128 bytes");

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

std::vector<ParamMeta> CommandFileReaderV1::readParams(std::ifstream &is, size_t count) {
    std::vector<ParamMeta> params;
    params.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        auto marshaled = read<ParameterMetadataMarshaled>(is);

        ParamMeta meta;
        meta.name = marshaled.name;
        meta.description = marshaled.description;
        meta.address = marshaled.memoryAddress;
        meta.rows = marshaled.rows;
        meta.columns = marshaled.columns;
        params.emplace_back(meta);
    }
    return params;
}

CommandFileReaderV1::Version CommandFileReaderV1::parseVersion(char *begin) {
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

const std::vector<ParamMeta> &CommandFileReaderV1::inputParamsMeta() const {
    return m_inputParams;
}

const std::vector<ParamMeta> &CommandFileReaderV1::outputParamsMeta() const {
    return m_outputParams;
}
