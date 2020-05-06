/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RHI/RHIDevice.h>

namespace Berserk {

    RHIDevice* RHIDevice::gDevice = nullptr;

    RHIDevice::RHIDevice() {
        if (gDevice == nullptr)
            gDevice = this;
        else {
            fprintf(stderr, "[BERSERK Device] Only single RHI Device could be set as singleton");
        }
    }

    RHIDevice& RHIDevice::getSingleton() {
        return *gDevice;
    }

}