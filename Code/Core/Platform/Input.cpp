/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/Input.h>
#include <stdio.h>

namespace Berserk {

    Input* Input::gInput = nullptr;

    Input::Input() {
        if (gInput) {
            fprintf(stderr, "[BERSERK Core] Only single Input System could be set as singleton");
            return;
        }

        gInput = this;
    }

    Input& Input::getSingleton() {
        return *gInput;
    }

}