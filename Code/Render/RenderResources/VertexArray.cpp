/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RHI/RHIDevice.h>
#include <RenderResources/VertexArray.h>
#include <RenderResources/VertexArrayBuilder.h>

namespace Berserk {
    namespace Render {

        VertexArray::VertexArray(struct VertexArrayBuilder &builder) {
            mName = std::move(builder.mName);
            mPrimitivesType = builder.mPrimitivesType;
            mIndicesCount = builder.mIndicesCount;
            mIndicesType = builder.mIndicesType;
            mVerticesCount = builder.mVerticesCount;
            mInstancesCount = builder.mInstancesCount;
            mDeclaration = std::move(builder.mDeclaration);
            mIndexBuffer = std::move(builder.mIndexBuffer);
            mVertexBuffers = std::move(builder.mVertexBuffers);

            auto& device = RHIDevice::getSingleton();
            mArrayObjectRHI = device.createArrayObject(mVertexBuffers, mIndexBuffer, mDeclaration->getRHI(), mPrimitivesType);
            BERSERK_COND_ERROR_RET(mArrayObjectRHI.isNotNull(), "Failed to create vertex array: %s", mName.data());
        }

        bool VertexArray::isInitialized() const {
            return mArrayObjectRHI.isNotNull();
        }

        bool VertexArray::isInitializedRHI() const {
            return mArrayObjectRHI.isNotNull();
        }

        CString VertexArray::getFriendlyName() const {
            return mName;
        }

        void VertexArray::draw(RHIDrawList &drawList) {
            BERSERK_COND_ERROR_RET(mArrayObjectRHI.isNotNull(), "Array object resource is not created");
            drawList.bindArrayObject(mArrayObjectRHI);

            if (hasIndices())
                drawList.drawIndexedInstanced(mIndicesType, mIndicesCount, mInstancesCount);
            else
                drawList.drawInstanced(mVerticesCount, mInstancesCount);
        }

    }
}