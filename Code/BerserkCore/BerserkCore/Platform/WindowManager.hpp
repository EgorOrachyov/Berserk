/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_WINDOWMANAGER_HPP
#define BERSERK_WINDOWMANAGER_HPP

#include <BerserkCore/Platform/Platform.hpp>

namespace Berserk {

    /** Handel platform specific window functionality */
    class WindowManager {
    public:
        virtual ~WindowManager() = default;
    };

}

#endif //BERSERK_WINDOWMANAGER_HPP