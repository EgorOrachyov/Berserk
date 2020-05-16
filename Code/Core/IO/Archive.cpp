/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/Archive.h>

namespace Berserk {

    void Archive::write(Berserk::uint8 v) {
        if (getType() == EArchiveType::Binary) {
            vwrite(&v, sizeof(uint8));
        }
    }

    void Archive::write(uint32 v) {
        if (getType() == EArchiveType::Binary) {
            vwrite(&v, sizeof(uint32));
        }
    }

    void Archive::write(uint64 v) {
        if (getType() == EArchiveType::Binary) {
            vwrite(&v, sizeof(uint64));
        }
    }

    void Archive::write(int32 v) {
        if (getType() == EArchiveType::Binary) {
            vwrite(&v, sizeof(int32));
        }
    }

    void Archive::write(float v) {
        if (getType() == EArchiveType::Binary) {
            vwrite(&v, sizeof(float));
        }
    }

    void Archive::write(bool v) {
        if (getType() == EArchiveType::Binary) {
            uint32 value = (v?1:0);
            vwrite(&value, sizeof(uint32));
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

    void Archive::read(Berserk::uint8 &v) {
        if (getType() == EArchiveType::Binary) {
            vread(&v, sizeof(uint8));
        }
    }

    void Archive::read(uint32 &v) {
        if (getType() == EArchiveType::Binary) {
            vread(&v, sizeof(uint32));
        }
    }

    void Archive::read(uint64 &v) {
        if (getType() == EArchiveType::Binary) {
            vread(&v, sizeof(uint64));
        }
    }

    void Archive::read(int32 &v) {
        if (getType() == EArchiveType::Binary) {
            vread(&v, sizeof(int32));
        }
    }

    void Archive::read(float &v) {
        if (getType() == EArchiveType::Binary) {
            vread(&v, sizeof(float));
        }
    }

    void Archive::read(bool &v) {
        if (getType() == EArchiveType::Binary) {
            uint32 value;
            vread(&value, sizeof(uint32));
            v = value != 0;
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