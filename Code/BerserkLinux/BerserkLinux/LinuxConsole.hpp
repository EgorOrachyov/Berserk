/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_LINUXCONSOLE_HPP
#define BERSERK_LINUXCONSOLE_HPP

#include <BerserkCore/IO/TextWriter.hpp>

namespace Berserk {

    class LinuxConsole: public TextWriter {
    public:
        explicit LinuxConsole(FILE* outputFile);
        ~LinuxConsole() override = default;
        void Write(size_t symbolsCount, const String::CharType *string) override;
        void Flush() override;

    private:
        FILE* mOutputFile;
    };

    class LinuxConsoleDummy: public TextWriter {
    public:
        ~LinuxConsoleDummy() override = default;
        void Write(size_t symbolsCount, const String::CharType *string) override {}
        void Flush() override {}
    };

}

#endif //BERSERK_LINUXCONSOLE_HPP