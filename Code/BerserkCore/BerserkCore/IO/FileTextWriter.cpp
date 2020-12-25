/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/IO/FileTextWriter.hpp>
#include <BerserkCore/Platform/FileSystem.hpp>
#include <BerserkCore/Assert.hpp>

namespace Berserk {

    FileTextWriter::FileTextWriter(PtrShared<Platform::File> &&file)
        : mFileHnd(std::move(file)) {
        BERSERK_ASSERT(mFileHnd.IsNotNull() && mFileHnd->IsOpen());
    }

    FileTextWriter::FileTextWriter(const String &filePath) {
        mFileHnd = std::move(Platform::FileSystem::OpenFile(filePath, Platform::File::Mode::Write));
        BERSERK_ASSERT(mFileHnd.IsNotNull() && mFileHnd->IsOpen());
    }

    void FileTextWriter::Write(size_t symbolsCount, const String::CharType *string) {
        BERSERK_ASSERT(mFileHnd.IsNotNull() && mFileHnd->IsOpen());
        mFileHnd->WriteBytes(string, sizeof(String::CharType) * symbolsCount);
    }

    void FileTextWriter::Flush() {
        BERSERK_ASSERT(mFileHnd.IsNotNull() && mFileHnd->IsOpen());
        mFileHnd->Flush();
    }
}
