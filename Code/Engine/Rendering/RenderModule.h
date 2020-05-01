/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RENDERMODULE_H
#define BERSERK_RENDERMODULE_H

#include <IModule.h>

#include <Rendering/VertexPolicy.h>

namespace Berserk {

    /**
     * @brief Rendering engine
     *
     * Rendering module processes all the rendering logic inside the engine.
     * This module allocates and tracks active rendering scenes, allocates and updates
     * render targets and render views, supports debug drawing and 2D drawing to canvas.
     *
     * This module main update is processed in the job system
     * in the separate thread. All unsafe requests to the rendering system
     * must be queued via special message queue.
     */
    class RenderModule : public IModule {
    protected:

        ~RenderModule() override;

        /** */
        void onPostInitialize() override;

        /** */
        void onPostFinalize() override;

        /** */
        void onPreUpdate() override;

        /** */
        void onPostUpdate() override;

    public:

        RenderModule();

        /** @return Default vertex policy factory */
        VertexPolicyFactory& getVertexPolicyFactory();

        /** @copydoc IModule::getModuleName() */
        const char *getModuleName() const override;

        /** @copydoc IModule::getModuleDescription() */
        const char *getModuleDescription() const override;

        /** @return Engine rendering module */
        static RenderModule& getSingleton();

    private:
        TPtrShared<VertexPolicyFactory> mVertexPolicyFactory;
    };

}

#endif //BERSERK_RENDERMODULE_H