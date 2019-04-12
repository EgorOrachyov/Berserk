//
// Created by Egor Orachyov on 22.02.2019.
//

#ifndef BERSERK_RENDERSYSTEM_H
#define BERSERK_RENDERSYSTEM_H

#include <Foundation/RenderBase.h>
#include <Foundation/IRenderSystem.h>
#include <Foundation/IPipelineScheduler.h>

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

        /** @copydoc IRenderSystem::() */
        void registerLightSource(LightSourceComponent* component) override;

        /** @copydoc IRenderSystem::() */
        void registerPrimitive(IPrimitiveComponent* component) override;

        /** @copydoc IRenderSystem::() */
        void unregisterLightSource(LightSourceComponent* component) override;

        /** @copydoc IRenderSystem::() */
        void unregisterPrimitive(IPrimitiveComponent* component) override;

        /** @copydoc IRenderSystem::() */
        LightSourceComponent* findLightSource(const char* name) override;

        /** @copydoc IRenderSystem::() */
        IPrimitiveComponent* findPrimitive(const char* name) override;

    protected:

        /** Global static region allocator */
        class IAllocator* mGenAllocator = nullptr;

        /** Rendering pipeline of the engine */
        class IPipelineScheduler* mPipelineScheduler = nullptr;

        ///////////////////// Light Sources info /////////////////////

        /** Registered spot light sources count */
        uint32 mSpotLightSourcesCount = 0;

        /** Registered point light sources count */
        uint32 mPointLightSourcesCount = 0;

        /** Registered directional light sources count */
        uint32 mDirLightSourcesCount = 0;

        /** Registered spot light sources */
        SpotLightComponent* mSpotLightSources = nullptr;

        /** Registered point light sources */
        PointLightComponent* mPointLightSources = nullptr;

        /** Registered directional light sources */
        DirectionalLightComponent* mDirLightSources = nullptr;

        ///////////////////// Geometry data info /////////////////////

        /** Registered static mesh components count */
        uint32 mStaticMeshesCount = 0;

        /** Registered static mesh components */
        StaticMeshComponent* mStaticMeshes = nullptr;

    };

} // namespace Berserk::RenderSystem

#endif //BERSERK_RENDERSYSTEM_H
