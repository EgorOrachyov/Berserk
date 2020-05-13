/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_STRING_H
#define BERSERK_STRING_H

#include <String/CString.h>
#include <Containers/TArray.h>

namespace Berserk {

    class String {
    public:

        static void split(const CString& source, const char* splitters, TArray<CString> &parts) {
            const char* seq = source.data();
            auto splittersCount = CStringUtility::length(splitters);

            while (*seq != '\0') {

                bool isSplitter = false;

                for (uint32 i = 0; i < splittersCount; i++) {
                    if (splitters[i] == *seq) {
                        isSplitter = true;
                        break;
                    }
                }

                if (isSplitter)
                    continue;

                uint32 wordLen = 1;
                const char* wordBegin = seq;
                seq += 1;
                bool word = true;

                while (*seq != '\0') {
                    for (uint32 i = 0; i < splittersCount; i++) {
                        if (splitters[i] == *seq) {
                            word = false;
                            seq += 1;
                            break;
                        }
                    }

                    if (!word)
                        break;

                    wordLen += 1;
                    seq += 1;
                }

                if (wordLen > 0) {
                    CString string(wordBegin, wordLen);
                    parts.move(string);
                }
            }
        }

    };

}

#endif //BERSERK_STRING_H
