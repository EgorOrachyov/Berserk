/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/IInput.h>
#include <stdio.h>

namespace Berserk {

    IInput* IInput::gInput = nullptr;

    IInput::IInput() {
        if (gInput) {
            fprintf(stderr, "[BERSERK Core] Only single Input System could be set as singleton");
            return;
        }

        gInput = this;
    }

    IInput& IInput::getSingleton() {
        return *gInput;
    }

}