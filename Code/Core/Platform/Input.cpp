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

    static Input* sInput = nullptr;

    Input::Input() {
        if (sInput == nullptr) {
            sInput = this;
        }
        else {
            fprintf(stderr, "[BERSERK Engine] Only single input device could be set as singleton");
        }
    }

    Input& Input::getSingleton() {
        return *sInput;
    }

}