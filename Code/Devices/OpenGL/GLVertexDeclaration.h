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

        bool create(const RHIVertexDeclarationDesc& vertexDeclarationDesc) {
            BERSERK_COND_ERROR_RET_VALUE(false, vertexDeclarationDesc.size() > 0, "Attempt to create empty vertex declaration");

            mElementsDeclarations = vertexDeclarationDesc;
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

                            BERSERK_COND_ERROR_RET_VALUE(false, vert.offset < bufferDeclaration.stride, "Invalid offset");
                            BERSERK_COND_ERROR_RET_VALUE(false, vert.stride == bufferDeclaration.stride, "Stride mismatched");
                            BERSERK_COND_ERROR_RET_VALUE(false, vert.iterating == bufferDeclaration.iterating, "Iterating mismatched");

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
            mLocationsUses = mElementsDeclarations.size();

            BERSERK_COND_ERROR_RET_VALUE(false, process == mLocationsUses, "All locations must be processed");

            return true;
        }

        const TArrayStatic<RHIVertexElement,RHIConst::MAX_VERTEX_ATTRIBUTES> &getVertexDeclarations() const {
            return mElementsDeclarations;
        }

        const TArrayStatic<BufferDeclaration,RHIConst::MAX_VERTEX_BUFFERS>& getBufferDeclarations() const {
            return mBufferDeclarations;
        }

    private:

        TArrayStatic<BufferDeclaration,RHIConst::MAX_VERTEX_BUFFERS> mBufferDeclarations;

    };

}

#endif //BERSERK_GLVERTEXDECLARATION_H