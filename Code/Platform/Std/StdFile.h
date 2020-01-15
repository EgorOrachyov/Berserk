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
#include <Errors.h>
#include <stdio.h>

namespace Berserk {

    class StdFile : public File {
    public:
        ~StdFile() override {
            if (mIsOpen) {
                close();
            }
        }

        bool isOpen() const override {
            return mIsOpen;
        }

        void close() override {
            fclose(mFileHandle);
            mIsOpen = false;
        }

        void flush() override {
            fflush(mFileHandle);
        }

        void seek(uint64 position) override {
            auto r = fseek(mFileHandle, position, SEEK_SET);
        }

        Error read(void *destination, uint64 size) override {
            if (!mCanRead) {
                return Error::FAILED_READ;
            }

            auto r = fread(destination, 1, size, mFileHandle);
            return Error::OK;
        }

        Error write(const void *source, int64 size) override {
            if (!mCanWrite) {
                return Error::FAILED_WRITE;
            }

            auto r = fwrite(source, 1, size, mFileHandle);
            return Error::OK;
        }

    private:
        FILE* mFileHandle;
        bool mCanRead: 1;
        bool mCanWrite: 1;
        bool mIsOpen: 1;
    };

}

#endif //BERSERK_STDFILE_H