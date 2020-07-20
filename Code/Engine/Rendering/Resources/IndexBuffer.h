/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_INDEXBUFFER_H
#define BERSERK_INDEXBUFFER_H

#include <Rendering/Resources/RenderResource.h>
#include <RHI/RHIResources.h>

namespace Berserk {
    namespace Rendering {

        /** Arbitrary index buffer for RHI index data access */
        class IndexBuffer : public RenderResource {
        public:
            ~IndexBuffer() override = default;
            /** @return Index resource buffer */
            const TPtrShared<RHIIndexBuffer> &getIndexBufferRHI() const { return mIndexBuffer; }
        protected:
            TPtrShared<RHIIndexBuffer> mIndexBuffer;
        };

    }
}

#endif //BERSERK_INDEXBUFFER_H
