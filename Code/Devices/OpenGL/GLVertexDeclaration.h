/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLVERTEXDECLARATION_H
#define BERSERK_GLVERTEXDECLARATION_H

#include <RHI/RHIResources.h>
#include <RHI/RHIDescs.h>
#include <GLDefinitions.h>
#include <GLErrors.h>
#include <TPtrShared.h>

namespace Berserk {

    class GLVertexDeclaration : public RHIVertexDeclaration {
    public:

        struct BufferDeclaration {
            EVertexIterating iterating;
            uint32 index;
            uint32 stride;
            TArrayStatic<uint32> vertices;
        };

        ~GLVertexDeclaration() override = default;

        void create(const RHIVertexDeclarationDesc& vertexDeclarationDesc) {
            BERSERK_COND_ERROR_FAIL(vertexDeclarationDesc.size() > 0, "Attempt to create empty vertex declaration");

            mVertexDeclarations = vertexDeclarationDesc;
            bool collectingData = true;
            uint32 currentIndex = 0;
            uint32 process = 0;

            while (collectingData) {
                collectingData = false;

                for (uint32 i = 0; i < vertexDeclarationDesc.size(); i++) {
                    const auto& declaration = vertexDeclarationDesc[i];

                    if (declaration.buffer == currentIndex) {
                        BufferDeclaration bufferDeclaration;
                        bufferDeclaration.index = currentIndex;
                        bufferDeclaration.stride = declaration.stride;
                        bufferDeclaration.iterating = declaration.iterating;

                        if (declaration.iterating == EVertexIterating::PerInstance)
                            mUsesInstancing = true;

                        for (uint32 j = i; j < vertexDeclarationDesc.size(); j++) {
                            const auto& vert = vertexDeclarationDesc[j];

                            if (vert.buffer != currentIndex)
                                continue;

                            BERSERK_COND_ERROR_FAIL(vert.offset < bufferDeclaration.stride, "Invalid offset");
                            BERSERK_COND_ERROR_FAIL(vert.stride == bufferDeclaration.stride, "Stride mismatched");
                            BERSERK_COND_ERROR_FAIL(vert.iterating == bufferDeclaration.iterating, "Iterating mismatched");

                            bufferDeclaration.vertices.add(j);

                            process += 1;
                        }

                        currentIndex += 1;
                        collectingData = true;

                        mBufferDeclarations.emplace(std::move(bufferDeclaration));
                    }

                    if (collectingData)
                        break;
                }
            }

            mBuffersUses = mBufferDeclarations.size();
            mLocationsUses = mVertexDeclarations.size();

            BERSERK_COND_ERROR_FAIL(process == mLocationsUses, "All locations must be processed");
        }

        bool compatible(const RHIVertexDeclarationDesc& other) const {
            if (mVertexDeclarations.size() != other.size())
                return false;

            for (uint32 i = 0; i < mVertexDeclarations.size(); i++) {
                bool equals = true;
                const auto& a = mVertexDeclarations[i];
                const auto& b = other[i];

                if (!(a == b))
                    return false;
            }

            return true;
        }

        const TArray<RHIVertexElement> &getVertexDeclarations() const {
            return mVertexDeclarations;
        }

        const TArray<BufferDeclaration>& getBufferDeclarations() const {
            return mBufferDeclarations;
        }

        static TPtrShared<GLVertexDeclaration> createDeclaration(const RHIVertexDeclarationDesc& vertexDeclarationDesc) {
            for (const auto& desc: mCachedDeclarations) {
                if (desc->compatible(vertexDeclarationDesc)) {
                    return desc;
                }
            }

            auto declaration = TPtrShared<GLVertexDeclaration>::make();
            declaration->create(vertexDeclarationDesc);
            mCachedDeclarations.add(declaration);
            return declaration;
        }

        static void clearCachedDeclarations() {
            mCachedDeclarations.clear();
        }

    private:

        TArray<RHIVertexElement> mVertexDeclarations;
        TArray<BufferDeclaration> mBufferDeclarations;

        /** Cache possible vertex declarations here */
        static TArray<TPtrShared<GLVertexDeclaration>> mCachedDeclarations;

    };

}

#endif //BERSERK_GLVERTEXDECLARATION_H