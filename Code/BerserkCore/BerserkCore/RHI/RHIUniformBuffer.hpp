/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHIUNIFORMBUFFER_H
#define BERSERK_RHIUNIFORMBUFFER_H

#include <BerserkCore/RHI/RHIDefinitions.hpp>
#include <BerserkCore/RHI/RHIResource.hpp>

namespace Berserk {

    class RHIUniformBuffer: public RHIResource {
    public:
        ~RHIUniformBuffer() override = default;
        uint32 GetBufferSize() const { return mSize; }
        EBufferUsage GetBufferUsage() const { return mBufferUsage; }

    protected:
        uint32 mSize;
        EBufferUsage mBufferUsage;
    };

}

#endif //BERSERK_RHIUNIFORMBUFFER_H