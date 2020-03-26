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

    static IInput* sInput = nullptr;

    IInput::IInput() {
        if (sInput == nullptr) {
            sInput = this;
        }
        else {
            fprintf(stderr, "[BERSERK Engine] Only single input system could be set as singleton");
        }
    }

    IInput& IInput::getSingleton() {
        return *sInput;
    }

}