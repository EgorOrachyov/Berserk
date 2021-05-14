/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkUnix/UnixConsole.hpp>

namespace Berserk {

    UnixConsole::UnixConsole(FILE *outputFile)
        : mOutputFile(outputFile) {

    }

    void UnixConsole::Write(size_t symbolsCount, const String::CharType *string) {
        fwrite(string, sizeof(String::CharType), symbolsCount, mOutputFile);
    }

    void UnixConsole::Flush() {
        fflush(mOutputFile);
    }

}