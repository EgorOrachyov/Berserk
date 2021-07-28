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

#include <BerserkPlatform/PlatformFile.hpp>
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

    PlatformFile::PlatformFile(const String &path, Mode mode) {
        mMode = mode;

        const char* nativeMode = GetMode(mode);
        mHND = fopen(path.GetStr_C(), nativeMode);

        if (mHND) {
            mName = std::move(FileSystem::GetFileNameFromPathUnix(path));
        }
    }

    PlatformFile::PlatformFile(PlatformFile &&other) noexcept
        : mName(std::move(other.mName)), mHND(other.mHND), mMode(other.mMode) {
        other.mHND = nullptr;
    }

    PlatformFile::~PlatformFile() {
        if (IsOpen()) {
            fclose(mHND);
            mHND = nullptr;
        }
    }

    void PlatformFile::Close() {
        if (IsOpen()) {
            fclose(mHND);
            mHND = nullptr;
        }
    }

    void PlatformFile::Flush() {
        if (!IsOpen())
            return;

        fflush(mHND);
    }

    void PlatformFile::Seek(uint64 position) {
        if (!IsOpen())
            return;

        fseek(mHND, (long) position, SEEK_SET);
    }

    bool PlatformFile::IsOpen() const {
        return mHND != nullptr;
    }

    bool PlatformFile::IsEof() const {
        if (!IsOpen())
            return false;

        return feof(mHND);
    }

    uint64 PlatformFile::GetSize() const {
        if (!IsOpen()) return 0;

        auto pos = ftell(mHND);
        fseek(mHND, 0, SEEK_END);

        auto size = ftell(mHND);
        fseek(mHND, pos, SEEK_SET);

        return static_cast<uint64>(size);
    }

    uint64 PlatformFile::GetPosition() const {
        if (!IsOpen())
            return 0;

        return ftell(mHND);;
    }

    File::Mode PlatformFile::GetAccessMode() const {
        return mMode;
    }

    const String &PlatformFile::GetFilename() const {
        return mName;
    }

    uint64 PlatformFile::ReadBytes(void *destination, uint64 size) {
        if (!IsOpen())
            return 0;

        return fread(destination, 1, size, mHND);
    }

    uint64 PlatformFile::WriteBytes(const void *source, uint64 size) {
        if (!IsOpen())
            return 0;

        return fwrite(source, 1, size, mHND);
    }

}