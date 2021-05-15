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

#ifndef BERSERK_FILE_HPP
#define BERSERK_FILE_HPP

#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/String/String.hpp>

namespace Berserk {

    enum class EFileStatus {
        NoError = 0,
        FailedRead,
        FailedWrite,
        FailedOffset
    };

    enum class EFileMode {
        Read = 0,
        Write
    };

    class File {
    public:
        virtual ~File() = default;
        virtual EFileStatus Offset(uint64 newPosition) = 0;
        virtual EFileStatus Read(void* destination, uint64 bytesToRead) const = 0;
        virtual EFileStatus Write(const void* source, uint64 bytesToWrite) = 0;
        virtual EFileMode GetFileMode() const = 0;
        virtual const String& GetOpenName() const = 0;
        virtual uint64 GetSize() const = 0;
        virtual uint64 GetPosition() const = 0;
        virtual bool CanRead() const { return GetFileMode() == EFileMode::Read; }
        virtual bool CanWrite() const { return GetFileMode() == EFileMode::Write; }
    };

}

#endif //BERSERK_FILE_HPP