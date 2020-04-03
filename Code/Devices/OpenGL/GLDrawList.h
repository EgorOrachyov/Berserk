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

    class GLDrawList : public RHIDrawList {
    public:


        ~GLDrawList() override {

        }

        void begin() override {

        }

        void end() override {

        }

        void bindWindow(ISystem::WINDOW_ID window) override {

        }

        void bindFramebuffer(const TPtrShared <RHIFramebuffer> &framebuffer) override {

        }

        void bindPipeline(const TPtrShared <RHIGraphicsPipeline> &pipeline) override {

        }

        void bindUniformSet(const TPtrShared <RHIUniformSet> &uniformSet) override {

        }

        void bindArrayObject(const TPtrShared <RHIArrayObject> &object) override {

        }

        void drawIndexed(EIndexType indexType, uint32 indexCount) override {

        }

    private:

        enum class ECommandType {
            BindSurface,
            BindFramebuffer,
            BindPipeline,
            BindUniformSet,
            BindArrayObject,
            DrawIndexed
        };

        struct Command {
            ECommandType type;

        };


    };

}

#endif //BERSERK_GLDRAWLIST_H