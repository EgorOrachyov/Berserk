/**********************************************************************************/
/* This file is part of LLGFx project                                             */
/* https://github.com/EgorOrachyov/LLGFx                                          */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2020 Egor Orachyov                                               */
/**********************************************************************************/

#ifndef BERSERK_SYSTEM_HPP
#define BERSERK_SYSTEM_HPP

#include <BerserkCore/Platform/Platform.hpp>

namespace Berserk {

    /** Abstracts underlying OS specifics */
    class System {
    public:
        virtual ~System() = default;
        virtual void onWarning() = 0;
        virtual void onError() = 0;
        virtual void onInfo() = 0;
        virtual class File* openFile() = 0;
        virtual class Directory* openDirectory() = 0;
        virtual class Library* openLibrary() = 0;
    };

}

#endif //BERSERK_SYSTEM_HPP