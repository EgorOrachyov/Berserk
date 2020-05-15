/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/Resources/IndexBufferUint32Static.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Rendering {

        IndexBufferUint32Static::IndexBufferUint32Static(uint32 count, const uint32 *data) {
            auto& device = RHIDevice::getSingleton();
            mIndexBuffer = device.createIndexBuffer(count * sizeof(uint32), EMemoryType::Static, data);
            if (mIndexBuffer.isNotNull()) linkResource();
        }

        IndexBufferUint32Static::IndexBufferUint32Static(const TArray<uint32> &indices) {
            auto& device = RHIDevice::getSingleton();
            mIndexBuffer = device.createIndexBuffer(indices.size() * sizeof(uint32), EMemoryType::Static, indices.data());
            if (mIndexBuffer.isNotNull()) linkResource();
        }

        IndexBufferUint32Static::~IndexBufferUint32Static() {
            unlinkResource();
        }

        bool IndexBufferUint32Static::isInitialized() const {
            return mIndexBuffer.isNotNull();
        }

        bool IndexBufferUint32Static::isInitializedRHI() const {
            return mIndexBuffer.isNotNull();
        }

        CString IndexBufferUint32Static::getFriendlyName() const {
            return "static-uint32-index-buffer";
        }
    }
}