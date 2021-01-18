/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHIUNIFORMBUFFER_HPP
#define BERSERK_RHIUNIFORMBUFFER_HPP

#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkRHI/RHIResource.hpp>
#include <BerserkCore/Memory/MemoryBuffer.hpp>

namespace Berserk {
    namespace RHI {

        class UniformBuffer: public Resource {
        public:

            struct Desc {
                Ref<MemoryBuffer> buffer;
                uint32 size;
                BufferUsage bufferUsage;
            };

            ~UniformBuffer() override = default;

            /** @return Type of the buffer memory */
            BufferUsage GetBufferUsage() const { return mBufferUsage; }

            /** @return Buffer total size in bytes */
            uint32 GetSize() const { return mSize; }

        protected:
            BufferUsage mBufferUsage;
            uint32 mSize = 0;
        };

    }
}


#endif //BERSERK_RHIUNIFORMBUFFER_HPP
