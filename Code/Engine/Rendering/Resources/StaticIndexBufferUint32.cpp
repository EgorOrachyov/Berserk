/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/Resources/StaticIndexBufferUint32.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Rendering {

        StaticIndexBufferUint32::StaticIndexBufferUint32(uint32 count, const uint32 *data) {
            auto& device = RHIDevice::getSingleton();
            mIndexBuffer = device.createIndexBuffer(count * sizeof(uint32), EMemoryType::Static, data);
            if (mIndexBuffer.isNotNull()) linkResource();
        }

        StaticIndexBufferUint32::StaticIndexBufferUint32(const TArray<uint32> &indices) {
            auto& device = RHIDevice::getSingleton();
            mIndexBuffer = device.createIndexBuffer(indices.size() * sizeof(uint32), EMemoryType::Static, indices.data());
            if (mIndexBuffer.isNotNull()) linkResource();
        }

        StaticIndexBufferUint32::~StaticIndexBufferUint32() {
            unlinkResource();
        }

        bool StaticIndexBufferUint32::isInitialized() const {
            return mIndexBuffer.isNotNull();
        }

        bool StaticIndexBufferUint32::isInitializedRHI() const {
            return mIndexBuffer.isNotNull();
        }

        CString StaticIndexBufferUint32::getFriendlyName() const {
            return "static-uint32-index-buffer";
        }
    }
}