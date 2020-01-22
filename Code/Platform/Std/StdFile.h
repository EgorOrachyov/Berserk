/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_STDFILE_H
#define BERSERK_STDFILE_H

#include <Platform/File.h>
#include <ErrorMacro.h>
#include <stdio.h>

namespace Berserk {

    class StdFile : public File {
    public:
        StdFile() {
            mCanWrite = false;
            mCanRead = false;
            mIsOpen = false;
        }
        StdFile(CString& path, EFileMode mode) : StdFile() {
            const char* openMode;

            switch (mode) {
                case EFileMode::Read:
                    openMode = "r";
                    mCanRead = true;
                    break;
                case EFileMode::Write:
                    openMode = "w";
                    mCanWrite = true;
                    break;
            }

            mFilePath = std::move(path);
            mFileHandle = fopen(mFilePath.data(), openMode);

            if (mFileHandle != nullptr) {
                mIsOpen = true;
            }
        }
        ~StdFile() override {
            if (mIsOpen) {
                close();
            }
        }

        void close() override {
            BERSERK_COND_ERROR_RET(mIsOpen, "File is not open [%s]", mFilePath.data())
            auto r = fclose(mFileHandle);
            mIsOpen = false;
            BERSERK_COND_ERROR_RET(r == 0, "Failed to close file [%s]", mFilePath.data())
        }
        void flush() override {
            BERSERK_COND_ERROR_RET(mIsOpen, "File is not open [%s]", mFilePath.data())
            fflush(mFileHandle);
        }
        void seek(uint64 position) override {
            BERSERK_COND_ERROR_RET(mIsOpen, "File is not open [%s]", mFilePath.data())
            fseek(mFileHandle, position, SEEK_SET);
        }
        void seekEnd(uint64 position) override {
            BERSERK_COND_ERROR_RET(mIsOpen, "File is not open [%s]", mFilePath.data())
            fseek(mFileHandle, position, SEEK_END);
        }
        bool isOpen() const override {
            return mIsOpen;
        }
        bool isEof() const override {
            BERSERK_COND_ERROR_RET_VALUE(false, mIsOpen, "File is not open [%s]", mFilePath.data())
            return feof(mFileHandle);
        }
        uint64 getSize() const override {
            BERSERK_COND_ERROR_RET_VALUE(0, mIsOpen, "File is not open [%s]", mFilePath.data())
            uint64 pos = ftell(mFileHandle);
            fseek(mFileHandle, 0, SEEK_END);
            uint64 size = ftell(mFileHandle);
            fseek(mFileHandle, pos, SEEK_SET);
            return size;
        }
        uint64 getPosition() const override {
            BERSERK_COND_ERROR_RET_VALUE(0, mIsOpen, "File is not open [%s]", mFilePath.data())
            return ftell(mFileHandle);
        }
        EFileMode getMode() const override {
            return mFlag;
        }
        const CString& getFilePath() const override {
            return mFilePath;
        }

        Error read(void *destination, uint64 size) override {
            BERSERK_COND_ERROR_RET_VALUE(Error::FAILED_READ, mIsOpen && mCanRead, "Cannot read from file [%s]", mFilePath.data())
            auto r = fread(destination, 1, size, mFileHandle);
            return (r == 0 ? Error::OK : Error::FAILED_READ);
        }
        Error write(const void *source, int64 size) override {
            BERSERK_COND_ERROR_RET_VALUE(Error::FAILED_WRITE, mIsOpen && mCanWrite, "Cannot write to file [%s]", mFilePath.data())
            auto r = fwrite(source, 1, size, mFileHandle);
            return (r == 0 ? Error::OK : Error::FAILED_WRITE);
        }
    private:
        CString mFilePath;
        FILE* mFileHandle = nullptr;
        EFileMode mFlag = EFileMode::Read;
        bool mCanRead: 1;
        bool mCanWrite: 1;
        bool mIsOpen: 1;
    };

}

#endif //BERSERK_STDFILE_H