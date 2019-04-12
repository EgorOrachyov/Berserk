//
// Created by Egor Orachyov on 22.02.2019.
//

#ifndef BERSERK_RENDERSYSTEM_H
#define BERSERK_RENDERSYSTEM_H

#include <Foundation/IRenderSystem.h>
#include <Foundation/RenderBase.h>

namespace Berserk::Render
{

    /**
     *
     */
    class RenderSystem final : public IRenderSystem, public RenderBase
    {
    public:

        /** Default System setup via initializer */
        explicit RenderSystem(const ISystemInitializer &systemInitializer);

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
        IAllocator* mGenAllocator = nullptr;

        ///////////////////// Light Sources info /////////////////////

        uint32 mTotalRegSpotLightComponets = 0;

        uint32 mTotalRegPointLightComponets = 0;

        uint32 mTotalRegDirectionalLightComponets = 0;

        SpotLightComponent* mRegSpotLightComponent = nullptr;

        PointLightComponent* mRegPointLightComponent = nullptr;

        DirectionalLightComponent* mRegDirectionalLightComponent = nullptr;

        ///////////////////// Geometry data info /////////////////////

        uint32 mTotalRegStaticMeshComponets = 0;

        StaticMeshComponent* mRegStaticMeshComponent = nullptr;

    };

} // namespace Berserk::RenderSystem

#endif //BERSERK_RENDERSYSTEM_H
