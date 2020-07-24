/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/System.h>

namespace Berserk {

    System* System::gSystem = nullptr;

    System::System() {
        if (gSystem) {
            fprintf(stderr, "[BERSERK Core] Only single System platform could be set as singleton");
            return;
        }

        gSystem = this;
    }

    CString System::Time::toString() const {
        char buffer[64];

        sprintf(buffer, "%i.%i.%2i %i:%i:%i",
                year, month+1, dayMonth+1, hour, min, sec);

        return buffer;
    }

    System& System::getSingleton() {
        return *gSystem;
    }
    
    ERenderDeviceType System::getDeviceTypeFromString(const CString &deviceName) {
        ERenderDeviceType type = ERenderDeviceType::Undefined;

        if (deviceName == "OpenGL")
            return ERenderDeviceType::OpenGL;

        return type;
    }
    
    CString System::getDeviceTypeAsString(ERenderDeviceType deviceType) {
        switch (deviceType) {
            case ERenderDeviceType::OpenGL:
                return "OpenGL";
            default:
                return "Undefined";
        }
    }

}