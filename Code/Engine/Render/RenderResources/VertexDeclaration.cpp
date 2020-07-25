/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RenderResources/VertexDeclaration.h>
#include <RenderResources/VertexDeclarationBuilder.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Render {

        VertexDeclaration::VertexDeclaration(struct VertexDeclarationBuilder &builder) {
            mName = std::move(builder.mName);
            mElementsIdx = std::move(builder.mElementsIdx);
            mBuffersIdx = std::move(builder.mBuffersIdx);
            mElements = std::move(builder.mElements);
            mBuffers = std::move(builder.mBuffers);

            auto& device = RHIDevice::getSingleton();
            mVertexDeclarationRHI = device.createVertexDeclaration(mElements);
            BERSERK_COND_ERROR_RET(mVertexDeclarationRHI.isNotNull(), "Failed to create vertex declaration: %s", mName.data());
        }

        bool VertexDeclaration::isInitialized() const {
            return mVertexDeclarationRHI.isNotNull();
        }

        bool VertexDeclaration::isInitializedRHI() const {
            return mVertexDeclarationRHI.isNotNull();
        }

        CString VertexDeclaration::getFriendlyName() const {
            return mName;
        }

        const RHIVertexElement& VertexDeclaration::getElement(const CString &name) const {
            auto ref = mElementsIdx.getPtr(name);
            BERSERK_COND_ERROR_FAIL(ref.isNotNull(), "No such element %s in declaration: %s", name.data(), mName.data());

            return mElements[*ref];
        }

        const VertexBufferInfo& VertexDeclaration::getBuffer(const CString &buffer) const {
            auto ref = mBuffersIdx.getPtr(buffer);
            BERSERK_COND_ERROR_FAIL(ref.isNotNull(), "No such buffer %s in declaration: %s", buffer.data(), mName.data());

            return mBuffers[*ref];
        }

        void VertexDeclaration::showDebugInfo() {
            printf("Declaration: %s\n", mName.data());

            for (auto& b: getBuffers()) {
                printf(" Buffer: Name=%s,Index=%u,Stride=%u,\n",
                       b.name.data(), b.index, b.stride);

                for (auto& name: b.elements) {
                    auto& element = getElement(name);
                    printf("  Element: Name=%s,Index=%u,Location=%u,Offset=%u,Stride=%u,Type=%s,TypeSize=%u\n",
                           name.data(), element.buffer, element.location, element.offset, element.stride,
                           RHIDefinitionsUtil::getVertexElementStringFromEnum(element.type),
                           RHIDefinitionsUtil::getVertexElementSize(element.type));
                }
            }
        }
    }

}