/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHIINDEXBUFFER_HPP
#define BERSERK_RHIINDEXBUFFER_HPP

#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkRHI/RHIResource.hpp>
#include <BerserkCore/Memory/MemoryBuffer.hpp>

namespace Berserk {
    namespace RHI {

        class IndexBuffer: public Resource {
        public:

            struct Desc {
                Ref<MemoryBuffer> buffer;
                uint32 elementsCount;
                IndexType indexType;
                BufferUsage bufferUsage;
            };

            ~IndexBuffer() override = default;

            /** @return Type of the buffer memory */
            BufferUsage GetBufferUsage() const { return mBufferUsage; }

            /** @return Total elements count */
            uint32 GetElementsCount() const { return mElementsCount; }

        protected:
            IndexType mIndexType;
            BufferUsage mBufferUsage;
            uint32 mElementsCount = 0;
        };

    }
}

#endif //BERSERK_RHIINDEXBUFFER_HPP