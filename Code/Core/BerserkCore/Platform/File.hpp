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

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Strings/String.hpp>

namespace Berserk {

    class File {
    public:

        enum class Mode {
            Read,
            Write,
            Append
        };

        virtual ~File() = default;

        /** Close file, no operation with file could not be done after this call */
        virtual void Close() = 0;

        /** Flush file content on the disk */
        virtual void Flush() = 0;

        /** Set position from the file beginning */
        virtual void Seek(size_t position) = 0;

        /** @return True if file opened */
        virtual bool IsOpen() const = 0;

        /** @return True if EOF reached */
        virtual bool IsEof() const = 0;

        /** @return File size (in bytes) */
        virtual size_t GetSize() const = 0;

        /** @return Current position in the file (in bytes) */
        virtual size_t GetPosition() const = 0;

        /** @return Open mode of the file */
        virtual Mode GetAccessMode() const = 0;

        /** @return Path of the file */
        virtual const String& GetFilename() const = 0;

        /**
         * Read file content into buffer
         * @param destination Where to store data
         * @param size Data amount in bytes
         *
         * @return Bytes read
         */
        virtual size_t ReadBytes(void* destination, size_t size) = 0;

        /**
         * Write buffer content into file
         * @param source Data to write into file
         * @param size Data amount in bytes
         *
         * @return Bytes written
         */
        virtual size_t WriteBytes(const void* source, size_t size) = 0;
    };

}

#endif //BERSERK_FILE_HPP