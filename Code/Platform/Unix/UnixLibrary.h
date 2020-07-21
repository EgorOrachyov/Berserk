/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_UNIXLIBRARY_H
#define BERSERK_UNIXLIBRARY_H

#include <Platform/Library.h>
#include <ErrorMacro.h>
#include <dlfcn.h>

namespace Berserk {

    class UnixLibrary : public Library {
    public:

        explicit UnixLibrary(CString& path) : mPath(std::move(path)) {
            mHandle = dlopen(mPath.data(), RTLD_LAZY);
            BERSERK_COND_ERROR_RET(mHandle != nullptr, "Failed to load library %s (%s)", mPath.data(), dlerror());
        }

        ~UnixLibrary() override {
            close();
        }

        EError loadSymbol(const CString &symbol, void *&handle) override {
            BERSERK_COND_ERROR_RET_VALUE(EError::UNAVAILABLE, isOpen(), "Attempt to access closed library")
            void* address = dlsym(mHandle, symbol.data());
            BERSERK_COND_ERROR_RET_VALUE(EError::FAILED_READ, address != nullptr, "Failed read symbol %s (%s)", address, dlerror());
            handle = address;
            return EError::OK;
        }

        EError close() override {
            if (!isOpen())
                return EError::OK;

            auto result = dlclose(mHandle);
            mHandle = nullptr;

            return result == 0? EError::OK : EError::FAILED_CLOSE;
        }

        bool isOpen() const override {
            return mHandle != nullptr;
        }

        const CString &loadPath() const override {
            return mPath;
        }

    private:

        CString mPath;
        void* mHandle = nullptr;

    };

}

#endif //BERSERK_UNIXLIBRARY_H