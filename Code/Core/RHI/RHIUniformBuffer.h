/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIUNIFORMBUFFER_H
#define BERSERK_RHIUNIFORMBUFFER_H

#include <RHI/RHIResource.h>

namespace Berserk {

    class RHIUniformBuffer : public RHIResource {
    public:
        ~RHIUniformBuffer() override = default;

        /** Updates specified buffer range with offset on GPU */
        virtual void update(uint32 range, uint32 offset, const void *data) = 0;

        /** @return Type of the buffer memory */
        EBufferUsage getBufferUsage() const { return mBufferUsage; }

        /** @return Buffer total size in bytes */
        uint32 getBufferSize() const { return mBufferSize; }

    protected:
        /** Type of the buffer memory */
        EBufferUsage mBufferUsage;

        /** Buffer total size in bytes */
        uint32 mBufferSize = 0;
    };

}

#endif //BERSERK_RHIUNIFORMBUFFER_H
