/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkUnix/UnixFile.hpp>
#include <BerserkCore/Platform/FileSystem.hpp>
#include <cstdio>

namespace Berserk {

    UnixFile::UnixFile(const String &path, Mode mode) {
        mMode = mode;

        const char* nativeMode = mode == Mode::Read? "r": "w";
        mHND = fopen(path.GetStr_C(), nativeMode);

        if (mHND) {
            mName = std::move(FileSystem::GetFileNameFromPathUnix(path));
        }
    }

    UnixFile::UnixFile(UnixFile &&other) noexcept
        : mName(std::move(other.mName)), mHND(other.mHND), mMode(other.mMode) {
        other.mHND = nullptr;
    }

    UnixFile::~UnixFile() {
        if (IsOpen()) {
            fclose(mHND);
            mHND = nullptr;
        }
    }

    void UnixFile::Close() {
        if (IsOpen()) {
            fclose(mHND);
            mHND = nullptr;
        }
    }

    void UnixFile::Flush() {
        if (!IsOpen())
            return;

        fflush(mHND);
    }

    void UnixFile::Seek(uint64 position) {
        if (!IsOpen())
            return;

        fseek(mHND, position, SEEK_SET);
    }

    bool UnixFile::IsOpen() const {
        return mHND != nullptr;
    }

    bool UnixFile::IsEof() const {
        if (!IsOpen())
            return false;

        return feof(mHND);
    }

    size_t UnixFile::GetSize() const {
        if (!IsOpen()) return 0;

        uint64 pos = ftell(mHND);
        fseek(mHND, 0, SEEK_END);

        uint64 size = ftell(mHND);
        fseek(mHND, pos, SEEK_SET);

        return size;
    }

    size_t UnixFile::GetPosition() const {
        if (!IsOpen())
            return 0;

        return ftell(mHND);;
    }

    File::Mode UnixFile::GetAccessMode() const {
        return mMode;
    }

    const String &UnixFile::GetFilename() const {
        return mName;
    }

    size_t UnixFile::ReadBytes(void *destination, size_t size) {
        if (!IsOpen())
            return 0;

        return fread(destination, 1, size, mHND);
    }

    size_t UnixFile::WriteBytes(const void *source, size_t size) {
        if (!IsOpen())
            return 0;

        return fwrite(source, 1, size, mHND);
    }

}