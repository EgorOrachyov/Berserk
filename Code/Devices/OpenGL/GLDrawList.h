/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLDRAWLIST_H
#define BERSERK_GLDRAWLIST_H

#include <RHI/RHIResources.h>
#include <GLDefinitions.h>
#include <GLErrors.h>
#include <GLDevice.h>
#include <TArray.h>

namespace Berserk {

    enum class ECommandType {
        BindSurface,
        BindFramebuffer,
        BindPipeline,
        BindUniformSet,
        BindArrayObject,
        DrawIndexed
    };

    class GLDrawList : public RHIDrawList {
    public:

        struct CmdDescription {
            ECommandType type;
            uint32 index;
        };

        struct CmdBindSurface {
            ISystem::WINDOW_ID window;
            Region2i viewport;
            Color4f clearColor;
        };

        struct CmdBindGraphicsPipeline {
            const RHIGraphicsPipeline* pipeline;
        };

        struct CmdBindArrayObject {
            const RHIArrayObject* arrayObject;
        };

        struct CmdDrawIndexed {
            EIndexType indexType;
            uint32 instancesCount;
            uint32 indexCount;
            uint32 baseOffset;
        };

    public:

        ~GLDrawList() override {
            destroy();
        }

        void create() {
            mListState = EDrawListState::Complete;
        }

        void destroy() {
            mCmdDescriptions.clear();
            mCmdBindSurface.clear();
            mCmdBindGraphicsPipeline.clear();
            mCmdBindArrayObject.clear();
            mCmdDrawIndexed.clear();
        }

        void begin() override {
            BERSERK_COND_ERROR_FAIL(mListState == EDrawListState::Complete, "Invalid list state");
            mListState = EDrawListState::Write;
            mCmdDescriptions.clear();
            mCmdBindSurface.clear();
            mCmdBindGraphicsPipeline.clear();
            mCmdBindArrayObject.clear();
            mCmdDrawIndexed.clear();
        }

        void end() override {
            BERSERK_COND_ERROR_FAIL(mListState == EDrawListState::Write, "Invalid list state");
            mListState = EDrawListState::Complete;
        }

        void bindWindow(ISystem::WINDOW_ID window, const Region2i &viewport, const Color4f &clearColor) override {
            auto cmdIndex = mCmdBindSurface.size();
            auto& cmd = mCmdBindSurface.emplace();
            cmd.window = window;
            cmd.viewport = viewport;
            cmd.clearColor = clearColor;
            auto& desc = mCmdDescriptions.emplace();
            desc.index = cmdIndex;
            desc.type = ECommandType::BindSurface;
        }

        void bindFramebuffer(const TPtrShared<RHIFramebuffer> &framebuffer) override {

        }

        void bindPipeline(const TPtrShared<RHIGraphicsPipeline> &pipeline) override {
            auto cmdIndex = mCmdBindGraphicsPipeline.size();
            auto& cmd = mCmdBindGraphicsPipeline.emplace();
            cmd.pipeline = pipeline.getPtr();
            auto& desc = mCmdDescriptions.emplace();
            desc.index = cmdIndex;
            desc.type = ECommandType::BindPipeline;
        }

        void bindUniformSet(const TPtrShared<RHIUniformSet> &uniformSet) override {

        }

        void bindArrayObject(const TPtrShared<RHIArrayObject> &object) override {
            auto cmdIndex = mCmdBindArrayObject.size();
            auto& cmd = mCmdBindArrayObject.emplace();
            cmd.arrayObject = object.getPtr();
            auto& desc = mCmdDescriptions.emplace();
            desc.index = cmdIndex;
            desc.type = ECommandType::BindArrayObject;
        }

        void drawIndexed(EIndexType indexType, uint32 indexCount) override {
            drawIndexedInstances(indexType, indexCount, 1);
        }

        void drawIndexedBaseOffset(EIndexType indexType, uint32 indexCount, uint32 baseOffset) override {
            auto cmdIndex = mCmdDrawIndexed.size();
            auto& cmd = mCmdDrawIndexed.emplace();
            cmd.indexType = indexType;
            cmd.indexCount = indexCount;
            cmd.baseOffset = baseOffset;
            cmd.instancesCount = 1;
            auto& desc = mCmdDescriptions.emplace();
            desc.index = cmdIndex;
            desc.type = ECommandType::DrawIndexed;
        }

        void drawIndexedInstances(EIndexType indexType, uint32 indexCount, uint32 instanceCount) override {
            auto cmdIndex = mCmdDrawIndexed.size();
            auto& cmd = mCmdDrawIndexed.emplace();
            cmd.indexType = indexType;
            cmd.indexCount = indexCount;
            cmd.baseOffset = 0;
            cmd.instancesCount = instanceCount;
            auto& desc = mCmdDescriptions.emplace();
            desc.index = cmdIndex;
            desc.type = ECommandType::DrawIndexed;
        }

        const TArray<CmdDescription> &getCmdDescriptions() const {
            return mCmdDescriptions;
        };

        const TArray<CmdBindSurface> &getCmdBindSurface() const {
            return mCmdBindSurface;
        };

        const TArray<CmdBindGraphicsPipeline> &getCmdBindGraphicsPipeline() const {
            return mCmdBindGraphicsPipeline;
        };

        const TArray<CmdBindArrayObject> &getCmdBindArrayObject() const {
            return mCmdBindArrayObject;
        };

        const TArray<CmdDrawIndexed> &getCmdDrawIndexed() const {
            return mCmdDrawIndexed;
        };

    private:

        TArray<CmdDescription>          mCmdDescriptions;
        TArray<CmdBindSurface>          mCmdBindSurface;
        TArray<CmdBindGraphicsPipeline> mCmdBindGraphicsPipeline;
        TArray<CmdBindArrayObject>      mCmdBindArrayObject;
        TArray<CmdDrawIndexed>          mCmdDrawIndexed;

    };

}

#endif //BERSERK_GLDRAWLIST_H