/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_UUID_H
#define BERSERK_UUID_H

#include <String/CStringStatic.h>
#include <IO/Archive.h>

namespace Berserk {

    class UUID {
    public:
        UUID() {
            for (auto& c: buffer) {
                c = 0;
            }
        }
        UUID(const UUID& other) {
            for (uint32 i = 0; i < SIZE; i++) {
                buffer[i] = other.buffer[i];
            }
        }
        UUID(UUID&& other) noexcept {
            for (uint32 i = 0; i < SIZE; i++) {
                buffer[i] = other.buffer[i];
                other.buffer[i] = 0;
            }
        }
        UUID(const char* string) : UUID() {
            using Util = TStringUtility<char, '\0'>;
            auto len = Util::length(string);

            if (len != SIZE * sizeof(uint32) * 2) return;

            for (auto& v: buffer) {
                uint32 r = 0x0;

                for (uint32 i = 0; i < 8; i++) {
                    char a = *string;
                    string += 1;

                    uint8 b;
                    r = r << 4u;

                    if (a >= '0' && a <= '9') {
                        b = a - '0';
                    }
                    else if (a >= 'a' && a <= 'f') {
                        b = a - 'a' + 10;
                    }
                    else {
                        *this = generateNull();
                        return;
                    }

                    r = (r | (b & 0xfu));
                }

                v = r;
            }
        }
        UUID(const CStringStatic& string) : UUID(string.data()) {

        }
        UUID& operator=(const UUID& other) {
            for (uint32 i = 0; i < SIZE; i++) {
                buffer[i] = other.buffer[i];
            }
            return *this;
        }
        UUID& operator=(UUID&& other) {
            for (uint32 i = 0; i < SIZE; i++) {
                buffer[i] = other.buffer[i];
            }
            return *this;
        }

        bool operator==(const UUID& other) const {
            auto result = true;
            for (uint32 i = 0; i < SIZE; i++) {
                result = result && (buffer[i] == other.buffer[i]);
            }
            return result;
        }

        bool operator!=(const UUID& other) const {
            auto result = false;
            for (uint32 i = 0; i < SIZE; i++) {
                result = result || (buffer[i] != other.buffer[i]);
            }
            return result;
        }

        bool operator<(const UUID& other) const {
            for (uint32 i = 0; i < SIZE; i++) {
                if (buffer[i] < other.buffer[i]) return true;
            }
            return false;
        }

        bool operator>(const UUID& other) const {
            for (uint32 i = 0; i < SIZE; i++) {
                if (buffer[i] > other.buffer[i]) return true;
            }
            return false;
        }

        bool operator<=(const UUID& other) const {
            for (uint32 i = 0; i < SIZE; i++) {
                if (buffer[i] <= other.buffer[i]) return true;
            }
            return false;
        }

        bool operator>=(const UUID& other) const {
            for (uint32 i = 0; i < SIZE; i++) {
                if (buffer[i] >= other.buffer[i]) return true;
            }
            return false;
        }

        bool isNull() const {
            uint32 s = 0;
            for (auto c: buffer) {
                s |= c;
            }
            return s == 0;
        }

        bool isNotNull() const {
            return !isNull();
        }

        /** @return String UUID representation */
        CStringStatic toString() {
            CStringStatic result;
            char* s = result.data();

            for (auto v : buffer) {
                char a[8];
                char h[16] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };

                for (int32 j = 0; j < 8; j++) {
                    a[j] = (v >> (28 - j * 4u)) & 0xfu;
                }

                for (uint32 j = 0; j < 8; j++) {
                    s[j] = h[a[j]];
                }

                s += 8;
            }

            *s = '\0';
            return result;
        }

        /** @return Num of bytes used by UUID */
        static uint32 size() { return sizeof(uint32) * SIZE; }
        /** Parse string as UUID: must contain 16 chars as hex UUID representation */
        static UUID fromString(const char* string) { UUID result(string); return result; }
        /** Generates random based UUID value */
        static UUID generate();
        /** Generates null (0..0) id */
        static UUID generateNull() { return {}; }

        friend Archive& operator<<(Archive& archive, const UUID& uuid) {
            for (auto v: uuid.buffer) {
                archive << v;
            }

            return archive;
        }

        friend Archive& operator>>(Archive& archive, UUID& uuid) {
            for (auto& v: uuid.buffer) {
                archive >> v;
            }

            return archive;
        }

    private:
        static const uint32 SIZE = 4;
        uint32 buffer[SIZE];
    };

}


#endif //BERSERK_UUID_H