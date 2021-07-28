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

#ifndef BERSERK_FILETEXTWRITER_HPP
#define BERSERK_FILETEXTWRITER_HPP

#include <BerserkCore/IO/TextWriter.hpp>
#include <BerserkCore/Platform/File.hpp>
#include <BerserkCore/Templates/SmartPointer.hpp>

namespace Berserk {

    /**
     * @brief File text writer
     *
     * Simple file wrapper to write textual data into platform fs files.
     */
    class FileTextWriter: public TextWriter {
    public:
        using TextWriter::Write;
        using TextWriter::WriteLine;
        using TextWriter::WriteF;
        using TextWriter::WriteFF;

        /** Create text writer from opened platform file */
        explicit FileTextWriter(SharedPtr<File> file);
        /** Create tex writer for provided file path */
        explicit FileTextWriter(const String &filePath);

        ~FileTextWriter() override = default;

        void Write(uint64 symbolsCount, const String::Char8u *string) override;
        void Flush() override;

    private:
        SharedPtr<File> mFileHnd;
    };

}

#endif //BERSERK_FILETEXTWRITER_HPP