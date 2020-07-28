/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIINDEXBUFFER_H
#define BERSERK_RHIINDEXBUFFER_H

#include <RHI/RHIResource.h>

namespace Berserk {

    class RHIIndexBuffer : public RHIResource {
    public:
        ~RHIIndexBuffer() override = default;

        /** Updates specified buffer range with offset on GPU */
        virtual void update(uint32 range, uint32 offset, const void *data) = 0;

        /** @return Type of the buffer memory */
        EBufferUsage getMemoryType() const { return mBufferMemoryType; }

        /** @return Buffer total size in bytes */
        uint32 getBufferSize() const { return mBufferSize; }

    protected:
        /** Type of the buffer memory */
        EBufferUsage mBufferMemoryType;

        /** Buffer total size in bytes */
        uint32 mBufferSize = 0;
    };

}

#endif //BERSERK_RHIINDEXBUFFER_H
