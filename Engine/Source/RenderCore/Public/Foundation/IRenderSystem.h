//
// Created by Egor Orachyov on 24.03.2019.
//

#ifndef BERSERK_IRENDERSYSTEM_H
#define BERSERK_IRENDERSYSTEM_H

#include <Systems/ISystem.h>
#include <Foundation/RenderPassInfo.h>
#include <Foundation/EffectsInfo.h>

namespace Berserk::RenderSystem
{
    using namespace EngineSystem;

    /**
     * Interface for the rendering system of the engine, Provides
     * exhaustive functionality and initializes its subsystems.
     *
     * Allows to register components, process them, capture screens.
     * Also allows to configure all the effects, available in the rendering engine
     */
    class ENGINE_API IRenderSystem : public ISystem
    {
    public:

        /** Provide minimal required interface for memory operations */
        GENERATE_CLASS_BODY(IRenderSystem);

        /** Do actually nothing */
        explicit IRenderSystem(const ISystemInitializer& systemInitializer) : ISystem(systemInitializer) {}

        /** Do actually nothing */
        ~IRenderSystem() override = default;

    public:

        /** Register a light source which will affect on scene lightning */
        virtual void registerLightSource(LightSourceComponent* component) = 0;

        /** Register a primitive component to be rendered */
        virtual void registerPrimitive(IPrimitiveComponent* component) = 0;

        /** Remove from light source rendering */
        virtual void unregisterLightSource(LightSourceComponent* component) = 0;

        /** Remove from rendering primitive rendering */
        virtual void unregisterPrimitive(IPrimitiveComponent* component) = 0;

        /**  @return Pointer to component, otherwise nullptr */
        virtual LightSourceComponent* findLightSource(const char* name) = 0;

        /**  @return Pointer to component, otherwise nullptr */
        virtual IPrimitiveComponent* findPrimitive(const char* name) = 0;

    public:

        /** Info about shadow mapping */
        DECLARE_PROPERTY_READ_REF(EffectInfoShadowMapping, EffectInfoShadowMapping);

        /** Info about deferred lightning */
        DECLARE_PROPERTY_READ_REF(EffectInfoDeferred, EffectInfoDeferred);

        /** Info about SSAO effect */
        DECLARE_PROPERTY_READ_REF(EffectInfoSSAO, EffectInfoSSAO);

        /** Info about light shafts effect */
        DECLARE_PROPERTY_READ_REF(EffectInfoLightShafts, EffectInfoLightShafts);

        /** Info about volumetric light effect */
        DECLARE_PROPERTY_READ_REF(EffectInfoVolumetricLight, EffectInfoVolumetricLight);

        /** Info about bloom effect */
        DECLARE_PROPERTY_READ_REF(EffectInfoBloom, EffectInfoBloom);

        /** Info about final tone mapping */
        DECLARE_PROPERTY_READ_REF(EffectInfoToneMapping, EffectInfoToneMapping);

    };

} // namespace Berserk

#endif //BERSERK_IRENDERSYSTEM_H
