/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MACOS_H
#define BERSERK_MACOS_H

#include <Platform/System.h>
#include <Std/StdAtomic.h>
#include <Std/StdFile.h>
#include <Std/StdMutex.h>
#include <GlfwSystem/GlfwInput.h>

#include <AllocPool.h>
#include <BuildOptions.h>

namespace Berserk {

    class macOS final : public System {
    public:

        macOS() noexcept
            : System(),
              mAllocFile(sizeof(StdFile)),
              mAllocMutex(sizeof(StdMutex)),
              mAllocAtomic(sizeof(StdAtomic)) {
        }

        ~macOS() override {
#ifdef BERSERK_DEBUG
            uint32 count;
            count = mAllocAtomic.getChunksAllocated();
            if (count != 0) {
                fprintf(stderr, "[Berserk Core] Lost allocated atomic [%u]\n", count);
            }
            count = mAllocFile.getChunksAllocated();
            if (count != 0) {
                fprintf(stderr, "[Berserk Core] Lost allocated file [%u]\n", count);
            }
            count = mAllocMutex.getChunksAllocated();
            if (count != 0) {
                fprintf(stderr, "[Berserk Core] Lost allocated mutex [%u]\n", count);
            }
#endif
        }

        Mutex& getErrorSyncMutex() override {
            return mErrorMutex;
        }

        TPtrUnique<File> openFile(CString path, EFileFlags flags) override {
            Guard guard(mAccessMutex);
            void* memory = mAllocFile.allocate(0);
            File* file = new (memory) StdFile();
            return TPtrUnique<File>(file, [](File* a){ ((macOS&)System::getSingleton()).deallocateFile(a); });
        }

        TPtrUnique<Mutex> createMutex() override {
            Guard guard(mAccessMutex);
            void* memory = mAllocMutex.allocate(0);
            Mutex* mutex = new (memory) StdMutex();
            return TPtrUnique<Mutex>(mutex, [](Mutex* a){ ((macOS&)System::getSingleton()).deallocateMutex(a); });
        }

        TPtrUnique<Atomic> createAtomic() override {
            Guard guard(mAccessMutex);
            void* memory = mAllocAtomic.allocate(0);
            Atomic* atomic = new (memory) StdAtomic();
            return TPtrUnique<Atomic>(atomic, [](Atomic* a){ ((macOS&)System::getSingleton()).deallocateAtomic(a); });
        }

    private:

        void deallocateFile(File* file) {
            Guard guard(mAccessMutex);
            mAllocFile.free(file);
        }

        void deallocateMutex(Mutex* mutex) {
            Guard guard(mAccessMutex);
            mAllocMutex.free(mutex);
        }

        void deallocateAtomic(Atomic* atomic) {
            Guard guard(mAccessMutex);
            mAllocAtomic.free(atomic);
        }

    private:
        StdMutex mErrorMutex;
        StdMutex mAccessMutex;
        AllocPool mAllocFile;
        AllocPool mAllocMutex;
        AllocPool mAllocAtomic;
        GlfwInput mInput;
    };

}

#endif //BERSERK_MACOS_H