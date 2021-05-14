/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
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
