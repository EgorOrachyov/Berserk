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

#include <BerserkCore/IO/FileTextWriter.hpp>
#include <BerserkCore/Platform/FileSystem.hpp>

namespace Berserk {

    FileTextWriter::FileTextWriter(SharedPtr<File> file)
        : mFileHnd(std::move(file)) {
        assert(mFileHnd.IsNotNull() && mFileHnd->IsOpen());
    }

    FileTextWriter::FileTextWriter(const String &filePath) {
        mFileHnd = std::move(FileSystem::OpenFile(filePath, File::Mode::Write));
        assert(mFileHnd.IsNotNull() && mFileHnd->IsOpen());
    }

    void FileTextWriter::Write(size_t symbolsCount, const String::CharType *string) {
        assert(mFileHnd.IsNotNull() && mFileHnd->IsOpen());
        mFileHnd->WriteBytes(string, sizeof(String::CharType) * symbolsCount);
    }

    void FileTextWriter::Flush() {
        assert(mFileHnd.IsNotNull() && mFileHnd->IsOpen());
        mFileHnd->Flush();
    }
}
