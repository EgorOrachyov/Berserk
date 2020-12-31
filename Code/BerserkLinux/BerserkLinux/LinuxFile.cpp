/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkLinux/LinuxFile.hpp>
#include <BerserkCore/Platform/FileSystem.hpp>
#include <cstdio>

namespace Berserk {
    namespace Platform {

        LinuxFile::LinuxFile(const String &path, Mode mode) {
            mMode = mode;

            const char* nativeMode = mode == Mode::Read? "r": "w";
            mHND = fopen(path.GetStr_C(), nativeMode);

            if (mHND) {
                mName = std::move(FileSystem::GetFileNameFromPathUnix(path));
            }
        }

        LinuxFile::LinuxFile(LinuxFile &&other) noexcept
            : mName(std::move(other.mName)), mHND(other.mHND), mMode(other.mMode)  {

            other.mHND = nullptr;
        }

        LinuxFile::~LinuxFile() {
            if (IsOpen()) {
                fclose(mHND);
                mHND = nullptr;
            }
        }

        void LinuxFile::Close() {
            if (IsOpen()) {
                fclose(mHND);
                mHND = nullptr;
            }
        }

        void LinuxFile::Flush() {
            if (!IsOpen())
                return;

            fflush(mHND);
        }

        void LinuxFile::Seek(uint64 position) {
            if (!IsOpen())
                return;

            fseek(mHND, position, SEEK_SET);
        }

        bool LinuxFile::IsOpen() const {
            return mHND != nullptr;
        }

        bool LinuxFile::IsEof() const {
            if (!IsOpen())
                return false;

            return feof(mHND);
        }

        size_t LinuxFile::GetSize() const {
            if (!IsOpen()) return 0;

            uint64 pos = ftell(mHND);
            fseek(mHND, 0, SEEK_END);

            uint64 size = ftell(mHND);
            fseek(mHND, pos, SEEK_SET);

            return size;
        }

        size_t LinuxFile::GetPosition() const {
            if (!IsOpen())
                return 0;

            return ftell(mHND);;
        }

        File::Mode LinuxFile::GetAccessMode() const {
            return mMode;
        }

        const String &LinuxFile::GetFilename() const {
            return mName;
        }

        size_t LinuxFile::ReadBytes(void *destination, size_t size) {
            if (!IsOpen())
                return 0;

            return fread(destination, 1, size, mHND);
        }

        size_t LinuxFile::WriteBytes(const void *source, size_t size) {
            if (!IsOpen())
                return 0;

            return fwrite(source, 1, size, mHND);
        }
    }
}