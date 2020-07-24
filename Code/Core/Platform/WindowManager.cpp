/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/WindowManager.h>

namespace Berserk {

    WindowManager* WindowManager::gWindowManager = nullptr;

    WindowManager::WindowManager() {
        if (gWindowManager) {
            fprintf(stderr, "[BERSERK Core] Only single WindowManager could be set as singleton");
            return;
        }

        gWindowManager = this;
    }

    WindowManager& WindowManager::getSingleton() {
        return *gWindowManager;
    }

}