/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/RenderModule.h>
#include <LogMacro.h>

namespace Berserk {

    BERSERK_DECLARE_MODULE(RenderModule);

    RenderModule::RenderModule() : IModule() {
        BERSERK_LOG_INFO("Register RenderModule (Rendering engine)");
    }

    RenderModule::~RenderModule() {

    }

    void RenderModule::onPostInitialize() {
        // Note: at this step RHI device already initialized
        // Note: put your components setup here

        // Engine vertex policy factory for default attributes layouts
        mVertexPolicyFactory = TPtrShared<VertexPolicyFactory>::make();
    }

    void RenderModule::onPostFinalize() {
        // Note: release in reverse order
        // Note: RHI device will de destroyed after the post-finalize step

        mVertexPolicyFactory.free();
    }

    void RenderModule::onPreUpdate() {

    }

    void RenderModule::onPostUpdate() {

    }

    VertexPolicyFactory& RenderModule::getVertexPolicyFactory() {
        return *mVertexPolicyFactory;
    }

    const char *RenderModule::getModuleName() const {
        static const char moduleName[] = "RenderModule";
        return moduleName;
    }

    const char *RenderModule::getModuleDescription() const {
        static const char moduleDescription[] =
                "Rendering module processes all the rendering logic inside the engine. "
                "This module allocates and tracks active rendering scenes, allocates and updates "
                "render targets and render views, supports debug drawing and 2D drawing to canvas. "
                "This module main update is processed in the job system "
                "in the separate thread. All unsafe requests to the rendering system "
                "must be queued via special message queue.";
        return moduleDescription;
    }

    RenderModule& RenderModule::getSingleton() {
        return BERSERK_MODULE_REFERENCE(RenderModule);
    }

}