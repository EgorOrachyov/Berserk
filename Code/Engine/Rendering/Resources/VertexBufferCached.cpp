/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/Resources/VertexBufferCached.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Rendering {

        VertexBufferCached::VertexBufferCached(uint32 size) {
            BERSERK_COND_ERROR_RET(size > 0, "Vertex buffer size must be more than 0");

            auto& device = RHIDevice::getSingleton();
            mVertexBuffer = device.createVertexBuffer(size, EMemoryType::Dynamic, nullptr);

            if (mVertexBuffer.isNotNull()) {
                mCachedData.resize(size);
                linkResource();
            }
        }

        VertexBufferCached::~VertexBufferCached() {
            unlinkResource();
        }

        bool VertexBufferCached::isInitialized() const {
            return mVertexBuffer.isNotNull();
        }

        bool VertexBufferCached::isInitializedRHI() const {
            return mVertexBuffer.isNotNull();
        }

        CString VertexBufferCached::getFriendlyName() const {
            return "VertexBufferCached";
        }

        void VertexBufferCached::write(uint32 range, uint32 offset, const void *data) {
            if (offset + range > mCachedData.size()) return;
            Memory::copy(mCachedData.data() + offset, data, range);
        }

        void VertexBufferCached::updateBufferOnGpu() {
            if (mVertexBuffer.isNotNull()) {
                mVertexBuffer->update(mCachedData.size(), 0, mCachedData.data());
            }
        }
    }
}