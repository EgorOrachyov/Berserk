/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/ISystem.h>

namespace Berserk {

    ISystem* ISystem::gSystem = nullptr;

    ISystem::ISystem() {
        if (gSystem) {
            fprintf(stderr, "[BERSERK Core] Only single System platform could be set as singleton");
            return;
        }

        gSystem = this;
    }

    ISystem& ISystem::getSingleton() {
        return *gSystem;
    }
    
    ERenderDeviceType ISystem::getDeviceTypeFromString(const CString &deviceName) {
        ERenderDeviceType type = ERenderDeviceType::Undefined;

        if (deviceName == "OpenGL")
            return ERenderDeviceType::OpenGL;

        return type;
    }
    
    CString ISystem::getDeviceTypeAsString(ERenderDeviceType deviceType) {
        switch (deviceType) {
            case ERenderDeviceType::OpenGL:
                return "OpenGL";
            default:
                return "Undefined";
        }
    }

}