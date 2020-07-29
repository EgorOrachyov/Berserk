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
#include <Containers/TArray.h>

namespace Berserk {

    enum class ECommandType {
        BindSurface,
        BindFramebuffer,
        BindPipeline,
        BindUniformSet,
        BindArrayObject,
        DrawIndexed,
        Draw,
        BeginTimeQuery,
        EndTimeQuery
    };

    class GLDrawList : public RHIDrawList {
    public:

        struct CmdDescription {
            ECommandType type;
            uint32 index;
        };

        struct CmdBindSurface {
            TPtrShared<Window> window;
            RHIWindowPassOptions options;
        };

        struct CmdBindFramebuffer {
            TPtrShared<RHIFramebuffer> framebuffer;
            RHIFramebufferPassOptions options;
        };

        struct CmdBindGraphicsPipeline {
            RHIGraphicsPipelineState pipeline;
        };

        struct CmdBindArrayObject {
            TPtrShared<RHIArrayObject> arrayObject;
        };

        struct CmdBindUniformSet {
            TPtrShared<RHIUniformSet> uniformSet;
        };

        struct CmdDrawIndexed {
            EIndexType indexType;
            uint32 instancesCount;
            uint32 indexCount;
            uint32 baseOffset;
        };

        struct CmdDraw {
            uint32 verticesCount;
            uint32 instancesCount;
            uint32 baseOffset;
        };

        struct CmdTimeQuery {
            TPtrShared<RHITimeQuery> query;
        };

    public:

        ~GLDrawList() override {
            destroy();
        }

        bool create() {
            mListState = EDrawListState::Complete;
            return true;
        }

        void destroy() {
            clear();
        }

        void begin() override {
            BERSERK_COND_ERROR_RET(mListState == EDrawListState::Complete, "Invalid list state");
            mListState = EDrawListState::Write;
            clear();
        }

        void end() override {
            BERSERK_COND_ERROR_RET(mListState == EDrawListState::Write, "Invalid list state");
            mListState = EDrawListState::Complete;
        }

        void bindWindow(TPtrShared<Window> window, const RHIWindowPassOptions& options) override {
            BERSERK_COND_ERROR_RET(mListState == EDrawListState::Write, "Invalid list state");
            auto cmdIndex = mCmdBindSurface.size();
            auto& cmd = mCmdBindSurface.emplace();
            cmd.window = std::move(window);
            cmd.options = options;
            auto& desc = mCmdDescriptions.emplace();
            desc.index = cmdIndex;
            desc.type = ECommandType::BindSurface;
        }

        void bindFramebuffer(const TPtrShared <RHIFramebuffer> &framebuffer, const RHIFramebufferPassOptions &options) override {
            BERSERK_COND_ERROR_RET(mListState == EDrawListState::Write, "Invalid list state");
            BERSERK_COND_ERROR_RET(framebuffer.isNotNull(), "An attempt to pass null framebuffer");
            auto cmdIndex = mCmdBindFramebuffer.size();
            auto& cmd = mCmdBindFramebuffer.emplace();
            cmd.framebuffer = framebuffer;
            cmd.options = options;
            auto& desc = mCmdDescriptions.emplace();
            desc.index = cmdIndex;
            desc.type = ECommandType::BindFramebuffer;
        }

        void bindPipeline(const RHIGraphicsPipelineState &pipeline) override {
            BERSERK_COND_ERROR_RET(mListState == EDrawListState::Write, "Invalid list state");
            auto cmdIndex = mCmdBindGraphicsPipeline.size();
            auto& cmd = mCmdBindGraphicsPipeline.emplace();
            cmd.pipeline = pipeline;
            auto& desc = mCmdDescriptions.emplace();
            desc.index = cmdIndex;
            desc.type = ECommandType::BindPipeline;
        }

        void bindUniformSet(const TPtrShared<RHIUniformSet> &uniformSet) override {
            BERSERK_COND_ERROR_RET(mListState == EDrawListState::Write, "Invalid list state");
            BERSERK_COND_ERROR_RET(uniformSet.isNotNull(), "An attempt to pass null uniform set");
            auto cmdIndex = mCmdBindUniformSet.size();
            auto& cmd = mCmdBindUniformSet.emplace();
            cmd.uniformSet = uniformSet;
            auto& desc = mCmdDescriptions.emplace();
            desc.index = cmdIndex;
            desc.type = ECommandType ::BindUniformSet;
        }

        void bindArrayObject(const TPtrShared<RHIArrayObject> &object) override {
            BERSERK_COND_ERROR_RET(mListState == EDrawListState::Write, "Invalid list state");
            BERSERK_COND_ERROR_RET(object.isNotNull(), "An attempt to pass null array object");
            auto cmdIndex = mCmdBindArrayObject.size();
            auto& cmd = mCmdBindArrayObject.emplace();
            cmd.arrayObject = object;
            auto& desc = mCmdDescriptions.emplace();
            desc.index = cmdIndex;
            desc.type = ECommandType::BindArrayObject;
        }

        void drawIndexed(EIndexType indexType, uint32 indexCount) override {
            BERSERK_COND_ERROR_RET(mListState == EDrawListState::Write, "Invalid list state");
            drawIndexedInstanced(indexType, indexCount, 1);
        }

        void drawIndexedBaseOffset(EIndexType indexType, uint32 indexCount, uint32 baseOffset) override {
            BERSERK_COND_ERROR_RET(mListState == EDrawListState::Write, "Invalid list state");
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

        void drawIndexedInstanced(EIndexType indexType, uint32 indexCount, uint32 instanceCount) override {
            BERSERK_COND_ERROR_RET(mListState == EDrawListState::Write, "Invalid list state");
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

        void draw(uint32 verticesCount) override {
            BERSERK_COND_ERROR_RET(mListState == EDrawListState::Write, "Invalid list state");
            auto cmdIndex = mCmdDraw.size();
            auto& cmd = mCmdDraw.emplace();
            cmd.verticesCount = verticesCount;
            cmd.instancesCount = 1;
            cmd.baseOffset = 0;
            auto& desc = mCmdDescriptions.emplace();
            desc.index = cmdIndex;
            desc.type = ECommandType::Draw;
        }

        void drawInstanced(uint32 verticesCount, uint32 instancesCount) override {
            BERSERK_COND_ERROR_RET(mListState == EDrawListState::Write, "Invalid list state");
            auto cmdIndex = mCmdDraw.size();
            auto& cmd = mCmdDraw.emplace();
            cmd.verticesCount = verticesCount;
            cmd.instancesCount = instancesCount;
            cmd.baseOffset = 0;
            auto& desc = mCmdDescriptions.emplace();
            desc.index = cmdIndex;
            desc.type = ECommandType::Draw;
        }

        void drawBaseOffset(uint32 verticesCount, uint32 baseOffset) override {
            BERSERK_COND_ERROR_RET(mListState == EDrawListState::Write, "Invalid list state");
            auto cmdIndex = mCmdDraw.size();
            auto& cmd = mCmdDraw.emplace();
            cmd.verticesCount = verticesCount;
            cmd.instancesCount = 1;
            cmd.baseOffset = baseOffset;
            auto& desc = mCmdDescriptions.emplace();
            desc.index = cmdIndex;
            desc.type = ECommandType::Draw;
        }

        void beginQuery(const TPtrShared <RHITimeQuery> &query) override {
            BERSERK_COND_ERROR_RET(mListState == EDrawListState::Write, "Invalid list state");
            BERSERK_COND_ERROR_RET(query.isNotNull(), "Passed null time query");
            auto cmdIndex = mCmdTimeQuery.size();
            auto& cmd = mCmdTimeQuery.emplace();
            cmd.query = query;
            auto& desc = mCmdDescriptions.emplace();
            desc.index = cmdIndex;
            desc.type = ECommandType::BeginTimeQuery;
        }

        void endQuery(const TPtrShared <RHITimeQuery> &query) override {
            BERSERK_COND_ERROR_RET(mListState == EDrawListState::Write, "Invalid list state");
            BERSERK_COND_ERROR_RET(query.isNotNull(), "Passed null time query");
            auto cmdIndex = mCmdTimeQuery.size();
            auto& cmd = mCmdTimeQuery.emplace();
            cmd.query = query;
            auto& desc = mCmdDescriptions.emplace();
            desc.index = cmdIndex;
            desc.type = ECommandType::EndTimeQuery;
        }

        const TArray<CmdDescription> &getCmdDescriptions() const {
            return mCmdDescriptions;
        };

        const TArray<CmdBindSurface> &getCmdBindSurface() const {
            return mCmdBindSurface;
        };

        const TArray<CmdBindFramebuffer> &getCmdBindFramebuffer() const {
            return mCmdBindFramebuffer;
        };

        const TArray<CmdBindGraphicsPipeline> &getCmdBindGraphicsPipeline() const {
            return mCmdBindGraphicsPipeline;
        };

        const TArray<CmdBindArrayObject> &getCmdBindArrayObject() const {
            return mCmdBindArrayObject;
        };

        const TArray<CmdBindUniformSet> &getCmdBindUniformSet() const {
            return mCmdBindUniformSet;
        };

        const TArray<CmdDrawIndexed> &getCmdDrawIndexed() const {
            return mCmdDrawIndexed;
        };

        const TArray<CmdDraw> &getCmdDraw() const {
            return mCmdDraw;
        };

        const TArray<CmdTimeQuery> &getCmdTimeQuery() const {
            return mCmdTimeQuery;
        }

        void clear() {
            mCmdDescriptions.clear();
            mCmdBindSurface.clear();
            mCmdBindFramebuffer.clear();
            mCmdBindGraphicsPipeline.clear();
            mCmdBindArrayObject.clear();
            mCmdBindUniformSet.clear();
            mCmdDrawIndexed.clear();
            mCmdDraw.clear();
            mCmdTimeQuery.clear();
        }

    private:

        TArray<CmdDescription>          mCmdDescriptions;
        TArray<CmdBindSurface>          mCmdBindSurface;
        TArray<CmdBindFramebuffer>      mCmdBindFramebuffer;
        TArray<CmdBindGraphicsPipeline> mCmdBindGraphicsPipeline;
        TArray<CmdBindArrayObject>      mCmdBindArrayObject;
        TArray<CmdBindUniformSet>       mCmdBindUniformSet;
        TArray<CmdDrawIndexed>          mCmdDrawIndexed;
        TArray<CmdDraw>                 mCmdDraw;
        TArray<CmdTimeQuery>            mCmdTimeQuery;

    };

}

#endif //BERSERK_GLDRAWLIST_H