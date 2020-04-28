/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/Archive.h>

namespace Berserk {

    void Archive::write(uint32 v) {
        if (getType() == EArchiveType::Binary) {
            vwrite(&v, sizeof(uint32));
        }
    }

    void Archive::write(float v) {
        if (getType() == EArchiveType::Binary) {
            vwrite(&v, sizeof(float));
        }
    }

    void Archive::write(const char *string) {
        if (getType() == EArchiveType::Binary) {
            auto length = CStringUtility::length(string);
            write(length);
            vwrite(string, length * sizeof(char));
        }
    }

    void Archive::write(const CString &string) {
        if (getType() == EArchiveType::Binary) {
            auto length = string.length();
            write(length);
            vwrite(string.data(), length * sizeof(char));
        }
    }

    void Archive::write(const CStringStatic &string) {
        if (getType() == EArchiveType::Binary) {
            auto length = string.length();
            write(length);
            vwrite(string.data(), length * sizeof(char));
        }
    }

    void Archive::read(uint32 &v) {
        if (getType() == EArchiveType::Binary) {
            vread(&v, sizeof(uint32));
        }
    }

    void Archive::read(float &v) {
        if (getType() == EArchiveType::Binary) {
            vread(&v, sizeof(float));
        }
    }

    void Archive::read(char *string, uint32 size) {
        if (getType() == EArchiveType::Binary) {
            uint32 len = 0;
            read(len);

            len = Math::min(len, size);
            vread(string, len);

            // Explicitly insert 0 symbol
            string[Math::min(len, size - 1)] = '\0';
        }
    }

    void Archive::read(CStringStatic &string) {
        read(string.data(), CStringStatic::CAPACITY);
    }

    void Archive::read(CString &string) {
        if (getType() == EArchiveType::Binary) {
            uint32 len = 0;
            read(len);

            uint32 capacity = len + 1;
            string.ensureCapacity(capacity);

            vread(string.data(), len);

            // Explicitly insert 0 symbol
            string.data()[len] = '\0';
        }
    }

}