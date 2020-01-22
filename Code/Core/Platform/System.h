/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SYSTEM_H
#define BERSERK_SYSTEM_H

#include <Platform/Mutex.h>
#include <Platform/Atomic.h>
#include <Platform/File.h>
#include <TPtrUnique.h>

namespace Berserk {

    class System {
    public:
        System();
        virtual ~System() = default;

        virtual Mutex&              getErrorSyncMutex() = 0;
        virtual TPtrUnique<File>    openFile(CString path, EFileMode mode) = 0;
        virtual TPtrUnique<Mutex>   createMutex() = 0;
        virtual TPtrUnique<Atomic>  createAtomic() = 0;

        // initialization
        // window query
        // video mode
        // icon set
        // dll loading
        // error logging

        static System& getSingleton();
    };

}

#endif //BERSERK_SYSTEM_H