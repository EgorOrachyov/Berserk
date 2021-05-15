/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
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
