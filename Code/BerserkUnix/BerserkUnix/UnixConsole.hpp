/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_UNIXCONSOLE_HPP
#define BERSERK_UNIXCONSOLE_HPP

#include <BerserkCore/IO/TextWriter.hpp>

namespace Berserk {

    class UnixConsole final: public TextWriter {
    public:
        explicit UnixConsole(FILE* outputFile);
        ~UnixConsole() override = default;
        void Write(size_t symbolsCount, const String::CharType *string) override;
        void Flush() override;

    private:
        FILE* mOutputFile;
    };

    class LinuxConsoleDummy final: public TextWriter {
    public:
        ~LinuxConsoleDummy() override = default;
        void Write(size_t symbolsCount, const String::CharType *string) override {}
        void Flush() override {}
    };

}

#endif //BERSERK_UNIXCONSOLE_HPP