/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VERTEXBUFFERCACHED_H
#define BERSERK_VERTEXBUFFERCACHED_H

#include <Rendering/Resources/VertexBuffer.h>
#include <Containers/TArray.h>
#include <RHI/RHIResources.h>

namespace Berserk {
    namespace Rendering {

        /** Vertex buffer GPU resource with cache on CPU (useful for dynamic vertex buffer update) */
        class VertexBufferCached : public VertexBuffer {
        public:
            VertexBufferCached(uint32 size);
            ~VertexBufferCached() override;

            bool isInitialized() const override;
            bool isInitializedRHI() const override;
            CString getFriendlyName() const override;

            void write(uint32 range, uint32 offset, const void* data);
            void updateBufferOnGpu();

            const TPtrShared<RHIVertexBuffer> &getVertexBufferRHI() const { return mVertexBuffer; }

        protected:
            TArray<uint8> mCachedData;
            TPtrShared<RHIVertexBuffer> mVertexBuffer;
        };

    }
}




#endif //BERSERK_VERTEXBUFFERCACHED_H
