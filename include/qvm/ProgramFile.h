#ifndef QVM_PROGRAMFILE_H
#define QVM_PROGRAMFILE_H

#include <qvm/Command.h>

/**
 * Версия протокола файла программы.
 */
struct Version {
    unsigned major = 0;
    unsigned minor = 0;
    unsigned bugFix = 0;
};

namespace ProgramFileV1 {
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

    /**
     * Команда с подсказками для диспетчера задач ВМ.
     */
    struct alignas(64) CommandWithHints : Command {
    };

    static_assert(sizeof(CommandWithHints) == 64, "The size of struct CommandWithHints must be 64 bytes");
}

#endif //QVM_PROGRAMFILE_H
