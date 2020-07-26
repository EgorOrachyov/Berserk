/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VERTEXDECLARATION_H
#define BERSERK_VERTEXDECLARATION_H

#include <RHI/RHIDescs.h>
#include <RHI/RHIDevice.h>
#include <RHI/RHIVertexDeclaration.h>
#include <Containers/TMap.h>
#include <Containers/TArray.h>
#include <RenderResources/RenderResource.h>

namespace Berserk {
    namespace Render {

        /** Describes single buffer */
        struct VertexBufferInfo {
            CString name;
            EVertexIterating iterating;
            uint32 index;
            uint32 stride;
            TArrayStatic<CString> elements;

            friend Archive& operator<<(Archive& archive, const VertexBufferInfo& bufferInfo) {
                archive << bufferInfo.name;
                archive << (uint32) bufferInfo.iterating;
                archive << bufferInfo.index;
                archive << bufferInfo.stride;
                archive << bufferInfo.elements;

                return archive;
            }

            friend Archive& operator>>(Archive& archive, VertexBufferInfo& bufferInfo) {
                archive >> bufferInfo.name;
                archive >> (uint32&) bufferInfo.iterating;
                archive >> bufferInfo.index;
                archive >> bufferInfo.stride;
                archive >> bufferInfo.elements;

                return archive;
            }
        };

        /**
         * @brief Vertex Declaration
         *
         * Describes vertex shader input elements layout
         * per vertex buffer. Supports elements data fetch by name.
         */
        class VertexDeclaration : public RenderResource {
        public:

            VertexDeclaration(class VertexDeclarationBuilder& builder);

            VertexDeclaration() = default;
            ~VertexDeclaration() override = default;

            bool isInitialized() const override;
            bool isInitializedRHI() const override;
            CString getFriendlyName() const override;

            /** @return True if contains element */
            bool hasElement(const CString& name) const { return mElementsIdx.contains(name); }

            /** @return Element info */
            const RHIVertexElement& getElement(const CString& name) const;

            /** @return True if contains buffer */
            bool hasBuffer(const CString& name) const { return mBuffersIdx.contains(name); }

            /** @return Buffer info */
            const VertexBufferInfo& getBuffer(const CString& buffer) const;

            /** @return Number of vertex buffers */
            uint32 getBuffersCount() const { return mBuffers.size(); }

            /** @return Number of vertex elements */
            uint32 getElementsCount() const { return mElements.size(); }

            /** @return Unique name for look-up */
            const CString& getName() const { return mName; }

            /** @return RHI elements */
            const TArray<RHIVertexElement> &getElements() const { return mElements; }

            /** @return Vertex buffers */
            const TArray<VertexBufferInfo> &getBuffers() const { return mBuffers; }

            /** @return RHI resource */
            const TPtrShared<RHIVertexDeclaration> &getRHI() const { return mVertexDeclarationRHI; }

            /** Print debug info about declaration */
            void showDebugInfo();

            /** Serialization to archive for caching operations */
            friend Archive& operator<<(Archive& archive, const VertexDeclaration& declaration) {
                archive << declaration.mName;
                archive << declaration.mElementsIdx;
                archive << declaration.mBuffersIdx;
                archive << declaration.mElements;
                archive << declaration.mBuffers;

                return archive;
            }

            /** Deserialization from archive for caching operations */
            friend Archive& operator>>(Archive& archive, VertexDeclaration& declaration) {
                archive >> declaration.mName;
                archive >> declaration.mElementsIdx;
                archive >> declaration.mBuffersIdx;
                archive >> declaration.mElements;
                archive >> declaration.mBuffers;

                auto& device = RHIDevice::getSingleton();
                auto& rhi = declaration.mVertexDeclarationRHI;
                rhi = device.createVertexDeclaration(declaration.mElements);

                BERSERK_COND_ERROR(rhi.isNotNull(), "Failed to create RHI declaration from archive: %s", declaration.mName.data());

                return archive;
            }

        private:

            CString mName;
            TMap<CString,uint32> mElementsIdx;
            TMap<CString,uint32> mBuffersIdx;
            TArray<RHIVertexElement> mElements;
            TArray<VertexBufferInfo> mBuffers;
            TPtrShared<RHIVertexDeclaration> mVertexDeclarationRHI;

        };

    }
}

#endif //BERSERK_VERTEXDECLARATION_H