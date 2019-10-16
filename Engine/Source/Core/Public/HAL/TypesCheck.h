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

#ifndef BERSERK_TYPESCHECK_H
#define BERSERK_TYPESCHECK_H

namespace Berserk {

#define SIZE_MESSAGE "Inappropriate type size for platform"

    static_assert(sizeof(float32) == 4, SIZE_MESSAGE);
    static_assert(sizeof(float64) == 8, SIZE_MESSAGE);

    static_assert(sizeof(int8)   == 1, SIZE_MESSAGE);
    static_assert(sizeof(int16)  == 2, SIZE_MESSAGE);
    static_assert(sizeof(int32)  == 4, SIZE_MESSAGE);
    static_assert(sizeof(int64)  == 8, SIZE_MESSAGE);

    static_assert(sizeof(uint8)  == 1, SIZE_MESSAGE);
    static_assert(sizeof(uint16) == 2, SIZE_MESSAGE);
    static_assert(sizeof(uint32) == 4, SIZE_MESSAGE);
    static_assert(sizeof(uint64) == 8, SIZE_MESSAGE);

#undef SIZE_MESSAGE

}

#endif //BERSERK_TYPESCHECK_H