/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkLinux/LinuxConsole.hpp>

namespace Berserk {

    LinuxConsole::LinuxConsole(FILE *outputFile)
        : mOutputFile(outputFile) {

    }

    void LinuxConsole::Write(size_t symbolsCount, const String::CharType *string) {
        fwrite(string, sizeof(String::CharType), symbolsCount, mOutputFile);
    }

    void LinuxConsole::Flush() {
        fflush(mOutputFile);
    }

}