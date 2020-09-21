/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkPlatform/PlatformFile.hpp>
#include <BerserkCore/LogMacro.hpp>

namespace Berserk {

    PlatformFile::PlatformFile(String name, EFileMode mode) {
        mOpenName = std::move(name);
        mMode = mode;

        const char* flags = mode == EFileMode::Read ? "r": mode == EFileMode::Write ? "w": "";
        mFileHnd = fopen(mOpenName.getCstr(), flags);

        if (!mFileHnd) {
            BERSERK_ERROR("Failed to open file");
        }
    }

    PlatformFile::~PlatformFile() {
        if (mFileHnd) {
            fclose(mFileHnd);
            mFileHnd = nullptr;
        }
    }

    EFileStatus PlatformFile::offset(uint64 newPosition) {
        if (!isSuccessfullyOpened()) return EFileStatus::FailedOffset;
        auto result = fseek(mFileHnd, newPosition, SEEK_SET);
        return result == 0? EFileStatus::NoError: EFileStatus::FailedOffset;
    }

    EFileStatus PlatformFile::read(void *destination, uint64 bytesToRead) const {
        if (!isSuccessfullyOpened()) return EFileStatus::FailedRead;
        auto result = fread(destination, 1, bytesToRead, mFileHnd);
        return result == bytesToRead? EFileStatus::NoError: EFileStatus::FailedOffset;
    }

    EFileStatus PlatformFile::write(const void *source, uint64 bytesToWrite) {
        if (!isSuccessfullyOpened()) return EFileStatus::FailedWrite;
        auto result = fwrite(source, 1, bytesToWrite, mFileHnd);
        return result == bytesToWrite? EFileStatus::NoError: EFileStatus::FailedOffset;
    }

    uint64 PlatformFile::getSize() const {
        if (!isSuccessfullyOpened()) return 0;
        uint64 pos = ftell(mFileHnd);
        fseek(mFileHnd, 0, SEEK_END);
        uint64 size = ftell(mFileHnd);
        fseek(mFileHnd, pos, SEEK_SET);
        return size;
    }

    uint64 PlatformFile::getPosition() const {
        if (!isSuccessfullyOpened()) return 0;
        return ftell(mFileHnd);;
    }

    EFileMode PlatformFile::getFileMode() const {
        return mMode;
    }

    const String &PlatformFile::getOpenName() const {
        return mOpenName;
    }
}