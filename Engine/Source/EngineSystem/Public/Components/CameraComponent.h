//
// Created by Egor Orachyov on 15.04.2019.
//

#ifndef BERSERK_CAMERACOMPONENT_H
#define BERSERK_CAMERACOMPONENT_H

#include <Components/SceneComponent.h>
#include <Math/MathInclude.h>

namespace Berserk::Engine
{

    /**
     * Main engine camera component class which defines the viewer camera properties
     * (how objects actually appear on the scene).
     *
     * Provides two kinds of projections: perspective and orthographic.
     * Allows to configure optic and image effects, such as motion blur, lens flares,
     * depth of field. Supports mechanism for frustum and distance culling.
     */
    class ENGINE_API CameraComponent : public SceneComponent
    {
    public:

        /** Provide minimal required interface for memory operations */
        GENERATE_CLASS_BODY(CameraComponent);

        /** Default view from (0,0,0) in z axis direction and y axis top side */
        explicit CameraComponent(const IObjectInitializer& objectInitializer);

        /** Nothing to free */
        ~CameraComponent() override = default;

    public:

        /** Initialize as Perspective camera view */
        void setProjection(float32 angle, float32 aspect, float32 near, float32 far);

        /** Initialize as Orthographic camera view */
        void setProjection(float32 left, float32 right, float32 bottom, float32 top, float32 near, float32 far);

        /** Recalculate internal camera data (in ref to SceneComponent transformation) */
        void onTransformUpdate() override;

        /** @return Relative position point */
        const Vec3f& getLocalPosition() const       { return mLocalPosition; }

        /** @return Relative direction vector (unit length) */
        const Vec3f& getLocalDirection() const      { return mLocalDirection; }

        /** @return Relative Up basis vector (unit length) */
        const Vec3f& getLocalUp() const             { return mLocalUp; }

        /** @return Absolute position point */
        const Vec3f& getWorldPosition() const       { return mWorldPosition; }

        /** @return Absolute direction vector (unit length) */
        const Vec3f& getWorldDirection() const      { return mWorldDirection; }

        /** @return Absolute Up basis vector (unit length) */
        const Vec3f& getWorldUp() const             { return mWorldUp; }

        /** @return View camera planes [enum via eFS] */
        const Plane* getViewPlanes() const          { return mFrustum.get(); }

        /** @return Projection matrix from last update */
        const Mat4x4f getProjection() const         { return mProjection; }

        /** @return View matrix from last update */
        const Mat4x4f getView() const               { return mView; }

    protected:

        /** Updates frustum planes */
        void updatePlanes();

    protected:

        /** Uses perspective projection */
        bool mIsPerspective : 1;

        /** Uses orthographic projection */
        bool mIsOrthographic : 1;

        /** View angle (between top and bottom frustum planes) */
        float32 mViewAngle = Math::PIf / 4.0f;

        /** Ratio of the view width to the height (r = w / h) */
        float32 mViewAspect = 16.0f / 9.0f;

        /** Near view distance */
        float32 mNearViewDistance = 0.001f;

        /** Far view distance (not seen after that) */
        float32 mFarViewDistance = 10.0f;

        /** Far view distance squared */
        float32 mFarViewDistanceSq = 100.0f;

        /** Left side of the camera [for orthographic projection] */
        float32 mOrthoViewLeft = 0.0f;

        /** Right side of the camera [for orthographic projection] */
        float32 mOrthoViewRight = 1.0f;

        /** Top side of the camera [for orthographic projection] */
        float32 mOrthoViewTop = 1.0f;

        /** Bottom side of the camera [for orthographic projection] */
        float32 mOrthoViewBottom = 0.0f;

        /** Relative position point */
        Vec3f mLocalPosition = Vec3f(0.0f);

        /** Relative direction vector (unit length) */
        Vec3f mLocalDirection = Vec3f::axisZ;

        /** Relative Up basis vector (unit length) */
        Vec3f mLocalUp = Vec3f::axisY;

        /** Absolute position point */
        Vec3f mWorldPosition = Vec3f(0.0f);

        /** Absolute direction vector (unit length) */
        Vec3f mWorldDirection = Vec3f::axisZ;

        /** Absolute Up basis vector (unit length) */
        Vec3f mWorldUp = Vec3f::axisY;

        /** Matrix used for perspective or orthographic projection */
        Mat4x4f mProjection;

        /** View matrix of this camera (reversed transformations) */
        Mat4x4f mView;

        /** View frustum (re-calculate each frame) */
        Frustum mFrustum;

    };

} // namespace Berserk::Engine

#endif //BERSERK_CAMERACOMPONENT_H