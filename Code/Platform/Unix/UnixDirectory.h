/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_UNIXDIRECTORY_H
#define BERSERK_UNIXDIRECTORY_H

#include <Platform/IDirectory.h>

#include <limits.h>
#include <stdlib.h>
#include <dirent.h>

namespace Berserk {

    class UnixDirectory : public IDirectory {
    public:
        explicit UnixDirectory(CString& path) : mPath(std::move(path)) {
            mDirHandle = opendir(mPath.data());

            if (mDirHandle != nullptr) {
                dirent* entry;

                while ((entry = readdir(mDirHandle)) != nullptr) {
                    auto& e = mEntries.emplace();
                    e.name = entry->d_name;

                    switch (entry->d_type) {
                        case DT_REG:
                            e.type = EDirEntryType::File;
                            break;
                        case DT_DIR:
                            e.type = EDirEntryType::Directory;
                            break;
                        case DT_LNK:
                            e.type = EDirEntryType::Link;
                            break;
                        default:
                            e.type = EDirEntryType::Undefined;
                    }
                }

                char buffer[PATH_MAX];
                realpath(mPath.data(), buffer);
                mRealPath = buffer;

                mIsOpen = true;
            }
        }

        ~UnixDirectory() override {
            if (mIsOpen) {
                close();
            }
        }

        void close() override {
            if (mDirHandle) {
                auto r = closedir(mDirHandle);
                mDirHandle = nullptr;
                mIsOpen = false;
                BERSERK_COND_ERROR_RET(r == 0, "Failed to close directory [%s]", mPath.data())
            }
        }

        bool isOpen() const override {
            return mIsOpen;
        }

        const CString &getPath() const override {
            return mPath;
        }

        const CString &getRealPath() const override {
            return mRealPath;
        }

        const TArray<Entry> &getEntries() const override {
            return mEntries;
        }

    private:
        DIR* mDirHandle = nullptr;
        bool mIsOpen = false;
        CString mPath;
        CString mRealPath;
        TArray<Entry> mEntries;
    };

}

#endif //BERSERK_UNIXDIRECTORY_H