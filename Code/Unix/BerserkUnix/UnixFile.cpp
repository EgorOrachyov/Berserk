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

#include <BerserkUnix/UnixFile.hpp>
#include <BerserkCore/Platform/FileSystem.hpp>
#include <cstdio>

namespace Berserk {

    static const char* GetMode(File::Mode mode) {
        switch (mode) {
            case File::Mode::Read:
                return "r";
            case File::Mode::Write:
                return "w";
            case File::Mode::Append:
                return "a";
            default:
                return nullptr;
        }
    }

    UnixFile::UnixFile(const String &path, Mode mode) {
        mMode = mode;

        const char* nativeMode = GetMode(mode);
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

    void UnixFile::Seek(size_t position) {
        if (!IsOpen())
            return;

        fseek(mHND, (long) position, SEEK_SET);
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