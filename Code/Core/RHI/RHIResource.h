/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIRESOURCE_H
#define BERSERK_RHIRESOURCE_H

#include <Image.h>
#include <TEnumMask.h>
#include <Containers/TArray.h>
#include <Containers/BinaryData.h>
#include <Containers/TArrayStatic.h>
#include <TPtrShared.h>
#include <PixelFormat.h>
#include <Math/Color4.h>
#include <Math/Region2i.h>
#include <Platform/System.h>
#include <Platform/Window.h>
#include <RHI/RHIDescs.h>
#include <RHI/RHIDefinitions.h>

namespace Berserk {

    /** Base RHI resource class */
    class RHIResource {
    public:
        /** Virtual to enable usage of shared pointers */
        virtual ~RHIResource() = default;
    };

}

#endif //BERSERK_RHIRESOURCE_H
