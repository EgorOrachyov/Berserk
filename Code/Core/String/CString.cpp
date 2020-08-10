/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <String/CString.h>
#include <Containers/TArray.h>

namespace Berserk {

    bool isSplitter(char s, const char* splitters, uint32 count) {
        for (uint32 i = 0; i < count; i++) {
            if (splitters[i] == s) {
                return true;
            }
        }

        return false;
    }

    void CString::split(const CString &source, const char *splitters, TArray<CString> &parts)  {
        const char* seq = source.data();
        auto splittersCount = CStringUtility::length(splitters);

        while (*seq != '\0') {

            if (!isSplitter(*seq, splitters, splittersCount)) {
                uint32 wordLen = 1;
                const char* wordBegin = seq;
                seq += 1;

                while (*seq != '\0' && !isSplitter(*seq, splitters, splittersCount)) {
                    wordLen += 1;
                    seq += 1;
                }

                if (wordLen > 0) {
                    CString string(wordBegin, wordLen);
                    parts.move(string);
                }

                if (*seq == '\0') {
                    return;
                }
            }

            seq += 1;

        }
    }

    void CString::splitWithQuotes(const CString &source, const char *splitters, TArray<CString> &parts) {
        const char* seq = source.data();
        auto splittersCount = CStringUtility::length(splitters);

        while (*seq != '\0') {

            if (*seq == '\"' || *seq == '\'') {
                uint32 wordLen = 0;
                seq += 1;
                const char* wordBegin = seq;

                while (*seq != '\0' && *seq != '\"' && *seq != '\'') {
                    wordLen += 1;
                    seq += 1;
                }

                CString string(wordBegin, wordLen);
                parts.move(string);

                if (*seq == '\0') {
                    return;
                }
            }
            else if (!isSplitter(*seq, splitters, splittersCount)) {
                uint32 wordLen = 1;
                const char* wordBegin = seq;
                seq += 1;

                while (*seq != '\0' && !isSplitter(*seq, splitters, splittersCount)) {
                    wordLen += 1;
                    seq += 1;
                }

                if (wordLen > 0) {
                    CString string(wordBegin, wordLen);
                    parts.move(string);
                }

                if (*seq == '\0') {
                    return;
                }
            }

            seq += 1;
        }
    }

}