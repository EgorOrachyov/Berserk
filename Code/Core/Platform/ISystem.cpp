/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/ISystem.h>

namespace Berserk {

    static ISystem* gSystem = nullptr;

    ISystem::ISystem() {
        if (gSystem == nullptr) {
            gSystem = this;
        }
        else {
            fprintf(stderr, "[BERSERK Engine] Only single System platform could be set as singleton");
        }
    }

    ISystem& ISystem::getSingleton() {
        return *gSystem;
    }

    void ISystem::initialize(CString windowName, const ISystem::VideoMode &videoMode, ERenderDeviceType deviceType) {
        getSingleton().vinitialize(std::move(windowName), videoMode, deviceType);
    }

    void ISystem::update() {
        getSingleton().vupdate();
    }

    void ISystem::finalize() {
        getSingleton().vfinalize();
    }

}