/**********************************************************************************/
/* This file is part of:                                                          */
/* Berserk Engine project                                                         */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018-2019 Egor Orachyov                                          */
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

#ifndef BERSERK_CRC32_H
#define BERSERK_CRC32_H

#include <HAL/Platform.h>

namespace Berserk
{

    /**
      * CRC hash generation for different types of input data
      * Supports only to uint32 hashing.
      */
    class Crc32
    {
    public:

        /** Hashing method type for different hashing applications */
        typedef uint32 (*Hashing)(const void* key);

        /**
         * CRC-32 hash function for float32 value
         *
         * @param value To be hashed
         * @return uint32 hash
         */
        static uint32 hash(float32 value);

        /**
         * CRC-32 hash function for int32 value
         *
         * @param value To be hashed
         * @return uint32 hash
         */
        static uint32 hash(int32 value);

        /**
         * CRC-32 hash function for float64 value
         *
         * @param value To be hashed
         * @return uint32 hash
         */
        static uint32 hash(float64 value);

        /**
         * CRC-32 hash function for int64 value
         *
         * @param value To be hashed
         * @return uint32 hash
         */
        static uint32 hash(int64 value);

        /**
         * CRC-32 hash function for chars array
         *
         * @param buffer Pointer to buffer
         * @param len Number of chars
         * @return uint32 hash
         */
        static uint32 hash(const char* buffer, uint32 len);

    };
}

#endif //BERSERK_CRC32_H
