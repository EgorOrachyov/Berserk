/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/System.h>

namespace Berserk {

    static ISystem* sOS = nullptr;

    ISystem::ISystem() {
        if (sOS == nullptr) {
            sOS = this;
        }
        else {
            fprintf(stderr, "[BERSERK Engine] Only single System platform could be set as singleton");
        }
    }

    ISystem& ISystem::getSingleton() {
        return *sOS;
    }

}