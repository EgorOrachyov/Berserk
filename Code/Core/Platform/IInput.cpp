/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/IInput.h>
#include <stdio.h>

namespace Berserk {

    static IInput* gInput = nullptr;

    IInput::IInput() {
        if (gInput == nullptr) {
            gInput = this;
        }
        else {
            fprintf(stderr, "[BERSERK Device] Only single input system could be set as singleton");
        }
    }

    IInput& IInput::getSingleton() {
        return *gInput;
    }

}