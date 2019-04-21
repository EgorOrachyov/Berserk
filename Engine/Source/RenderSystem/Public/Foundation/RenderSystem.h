//
// Created by Egor Orachyov on 22.02.2019.
//

#ifndef BERSERK_RENDERSYSTEM_H
#define BERSERK_RENDERSYSTEM_H

#include <Foundation/IRenderSystem.h>
#include <Foundation/IPipelineScheduler.h>
#include <Queue/RenderQueue.h>

namespace Berserk::Render
{

    /**
     * IMain implementation of the rendering system of the engine.
     * Provides exhaustive functionality and initializes its subsystems.
     *
     * Allows to register components, process them, capture screens.
     * Also allows to configure all the effects, available in the rendering engine
     */
    class RenderSystem final : public IRenderSystem, public RenderBase
    {
    public:

        /** To store allocated stages (to transform pipeline in runtime mode) */
        typedef ArrayList<IPipelineStage*> PipelineStages;

        /** Default memory operations */
        GENERATE_CLASS_BODY(RenderSystem);

        /** Default System setup via initializer */
        explicit RenderSystem(const ISystemInitializer &systemInitializer);

        /** Release all used resources */
        ~RenderSystem() override;

    public:

        /** @copydoc IRenderSystem::() */
        void initialize() override;

        /** @copydoc IRenderSystem::() */
        void preUpdate() override;

        /** @copydoc IRenderSystem::() */
        void update() override;

        /** @copydoc IRenderSystem::() */
        void postUpdate() override;

        /** @copydoc IRenderSystem::() */
        void destroy() override;

    public:

        /** Register a light source which will affect on scene lightning */
        void registerComponent(SpotLightComponent *component) override;

        /** Register a light source which will affect on scene lightning */
        void registerComponent(PointLightComponent *component) override;

        /** Register a light source which will affect on scene lightning */
        void registerComponent(DirectionalLightComponent *component) override;

        /** Register a primitive component to be rendered */
        void registerComponent(StaticMeshComponent *component) override;

    protected:

        static const uint32 INITIAL_STAGES_COUNT = 16;

        /** Data needed for pipeline stages */
        RenderPassInfo mRenderPass;

        /** Global static region allocator */
        class IAllocator* mGenAllocator = nullptr;

        /** Rendering pipeline of the engine */
        class IPipelineScheduler* mPipelineScheduler = nullptr;

        /** Render queue to stores all the nodes to render in the current frame */
        class RenderQueue* mRenderQueue = nullptr;

        /** Main render engine frame buffer */
        class IFrameBuffer* mFrameBuffer = nullptr;

        /** Stores all the pipeline stages, which could be called by the render */
        class ArrayList<IPipelineStage*>* mPipelineStages = nullptr;

        ///////////////////// Light Sources info /////////////////////

        /** Registered spot light sources count */
        uint32 mSpotLightSourcesCount;

        /** Registered point light sources count */
        uint32 mPointLightSourcesCount;

        /** Registered directional light sources count */
        uint32 mDirLightSourcesCount;

        /** Registered spot light sources */
        class SpotLightComponent* mSpotLightSources = nullptr;

        /** Registered point light sources */
        class PointLightComponent* mPointLightSources = nullptr;

        /** Registered directional light sources */
        class DirectionalLightComponent* mDirLightSources = nullptr;

        ///////////////////// Geometry data info /////////////////////

        /** Registered static mesh components count */
        uint32 mStaticMeshesCount;

        /** Registered static mesh components */
        class StaticMeshComponent* mStaticMeshes = nullptr;

    };

} // namespace Berserk::RenderSystem

#endif //BERSERK_RENDERSYSTEM_H
