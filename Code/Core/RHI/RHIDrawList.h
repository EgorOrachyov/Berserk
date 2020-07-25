/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIDRAWLIST_H
#define BERSERK_RHIDRAWLIST_H

#include <RHI/RHIResource.h>
#include <RHI/RHIUniformSet.h>
#include <RHI/RHIArrayObject.h>
#include <RHI/RHIFramebuffer.h>
#include <RHI/RHIGraphicsPipeline.h>

namespace Berserk {

    class RHIDrawList : public RHIResource {
    public:
        ~RHIDrawList() override = default;

        /** Begin render list commands setup */
        virtual void begin() = 0;

        /** End render list commands setup */
        virtual void end() = 0;

        /** Bind platform window as a render target */
        virtual void bindWindow(TPtrShared<Window> window, const Region2i &viewport, const Color4f &clearColor) = 0;

        /** Bind platform window as a render target */
        virtual void bindWindow(TPtrShared<Window> window, const Region2i &viewport, const Color4f &clearColor, float clearDepth, int32 clearStencil) = 0;

        /** Bind frame buffer as render target for the list rendering */
        virtual void bindFramebuffer(const TPtrShared<RHIFramebuffer> &framebuffer, const Region2i &viewport) = 0;

        /** Bind graphics pipeline to configure rendering process */
        virtual void bindPipeline(const TPtrShared<RHIGraphicsPipeline> &pipeline) = 0;

        /** Bind uniform set to the pipeline */
        virtual void bindUniformSet(const TPtrShared<RHIUniformSet> &uniformSet) = 0;

        /** Bind array object as data to be rendered */
        virtual void bindArrayObject(const TPtrShared<RHIArrayObject> &object) = 0;

        /** Draw bound array object indexed */
        virtual void drawIndexed(EIndexType indexType, uint32 indexCount) = 0;

        /** Draw bound array object indexed with instancing and base offset for each indexed vertex */
        virtual void drawIndexedBaseOffset(EIndexType indexType, uint32 indexCount, uint32 baseOffset) = 0;

        /** Draw bound array object indexed with instancing */
        virtual void drawIndexedInstances(EIndexType indexType, uint32 indexCount, uint32 instanceCount) = 0;

        /** Draw bound array object with raw vertex count */
        virtual void draw(uint32 verticesCount) = 0;

        /** Draw bound array object instanced with raw vertex count */
        virtual void drawInstanced(uint32 verticesCount, uint32 instancesCount) = 0;

        /** Draw bound array object with raw vertex count with offset for the first vertex  */
        virtual void drawBaseOffset(uint32 verticesCount, uint32 baseOffset) = 0;

        /** @return Current state of the list */
        EDrawListState getDrawListState() const { return mListState; }

    protected:
        /** Current state of the list */
        EDrawListState mListState;
    };

}

#endif //BERSERK_RHIDRAWLIST_H
