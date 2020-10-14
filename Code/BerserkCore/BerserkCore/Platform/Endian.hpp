/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_ENDIAN_HPP
#define BERSERK_ENDIAN_HPP

#include <BerserkCore/Platform/Platform.hpp>

namespace Berserk {

    enum class EBytesOrder: uint8 {
        LittleEndian,
        BigEndian
    };

    class Endian {
    public:

        static bool IsLittleEndian() {
            union {
                uint8 bytesArray[sizeof(uint32)];
                uint32 bytesType = 0x07;
            } test;

            return test.bytesArray[0] == 0x07;
        }

        static bool IsBigEndian() {
            union {
                uint8 bytesArray[sizeof(uint32)];
                uint32 bytesType = 0x070000000;
            } test;

            return test.bytesArray[0] == 0x07;
        }

        static bool NeedToReorder(EBytesOrder bytesOrder) {
            return (bytesOrder == EBytesOrder::LittleEndian && IsBigEndian()) ||
                   (bytesOrder == EBytesOrder::BigEndian && IsLittleEndian());
        }

        template<typename T>
        static T Reorder(T v) {
            union {
                T a;
                uint8 b[sizeof(T)];
            } source, dest;

            source.a = v;

            for (int64 i = 0; i < sizeof(T); i++) {
                dest.b[sizeof(T) - i - 1] = source.b[i];
            }

            return dest.a;
        }

    };

}

#endif //BERSERK_ENDIAN_HPP
