//
// Created by Egor Orachyov on 03.04.2019.
//

#ifndef BERSERK_DIRECTIONALLIGHTCOMPONENT_H
#define BERSERK_DIRECTIONALLIGHTCOMPONENT_H

#include <Components/LightComponent.h>

namespace Berserk::Engine
{

    /**
     * Light source component with parallel light rays. Could be used for emulating sun light.
     * Handel only on shadow map buffer. Has adaptive view space view volume calculation relatively
     * to current camera position, what allows to optimize shadow map generation.
     */
    class DirectionalLightComponent : public LightComponent
    {
    public:

        GENERATE_CLASS_BODY(DirectionalLightComponent);

        /** Default */
        explicit DirectionalLightComponent(const IObjectInitializer& objectInitializer);

        /** Default */
        ~DirectionalLightComponent() override = default;

    public:

        /** @return Relative direction vector (unit length) */
        const Vec3f& getLocalDirection() const      { return mLocalDirection; }

        /** @return Relative Up basis vector (unit length) */
        const Vec3f& getLocalUp() const             { return mLocalUp; }

        /** @return Absolute direction vector (unit length) */
        const Vec3f& getWorldDirection() const      { return mWorldDirection; }

        /** @return Absolute Up basis vector (unit length) */
        const Vec3f& getWorldUp() const             { return mWorldUp; }

        /** @return View camera planes [enum via eFS] */
        const Plane* getViewPlanes() const          { return mShadowView.get(); }

        /** @return Projection matrix from last update */
        const Mat4x4f getProjection() const         { return mProjection; }

        /** @return View matrix from last update */
        const Mat4x4f getView() const               { return mView; }

        /** Recalculate internal source data (in ref to SceneComponent transformation) */
        void onTransformUpdate() override;

        /** Updates */
        virtual void calculateShadowVolume(const Vec3f* cameraFrustumPoints);

    public: /*for debug*/

        friend class ::Berserk::Render::RenderSystem;

        /** View volume of this light source, computed each frame relatively to the current rendering camera */
        Frustum mShadowView;

        /** Orthographic projection used to create shadow map */
        Mat4x4f mProjection;

        /** From world to light source view space matrix */
        Mat4x4f mView;

        /** From light source view space to world space matrix */
        Mat4x4f mFromViewToWorld;

        /** Relative direction vector (unit length) */
        Vec3f mLocalDirection = Vec3f::axisZ;

        /** Relative Up basis vector (unit length) */
        Vec3f mLocalUp = Vec3f::axisY;

        /** Absolute direction vector (unit length) */
        Vec3f mWorldDirection = Vec3f::axisZ;

        /** Absolute Up basis vector (unit length) */
        Vec3f mWorldUp = Vec3f::axisY;

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_DIRECTIONALLIGHTCOMPONENT_H