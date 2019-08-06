//
// Created by Egor Orachyov on 2019-08-06.
//

#ifndef BERSERK_CAMERABASE_H
#define BERSERK_CAMERABASE_H

#include <Misc/Types.h>
#include <Misc/Bits.h>
#include <Misc/UsageDescriptors.h>
#include <Math/MathInclude.h>

namespace Berserk
{

    /** Perspective camera setup params */
    struct ENGINE_API PerspectiveSettings
    {
    public:

        /** View angle (between top and bottom frustum planes) */
        float32 ViewAngle = Math::PIf / 4.0f;

        /** Ratio of the view width to the height (r = w / h) */
        float32 ViewAspect = 16.0f / 9.0f;

        /** Near view distance */
        float32 NearViewDistance = 0.001f;

        /** Far view distance (not seen after that) */
        float32 FarViewDistance = 10.0f;

    };


    /** Orthographic camera setup params */
    struct ENGINE_API OrthographicSettings
    {
    public:

        /** Near view distance */
        float32 ViewNear = 0.001f;

        /** Far view distance (not seen after that) */
        float32 ViewFar = 10.0f;

        /** Left side of the camera [for orthographic projection] */
        float32 ViewLeft = 0.0f;

        /** Right side of the camera [for orthographic projection] */
        float32 ViewRight = 1.0f;

        /** Top side of the camera [for orthographic projection] */
        float32 ViewTop = 1.0f;

        /** Bottom side of the camera [for orthographic projection] */
        float32 ViewBottom = 0.0f;

    };


    /** Defines camera space for proper world view */
    struct ENGINE_API CameraSpace
    {
    public:

        /** Position point */
        Vec3f Position = Vec3f(0.0f);

        /** Direction vector (unit length) */
        Vec3f Direction = Vec3f::axisZ;

        /** Up basis vector (unit length) */
        Vec3f Up = Vec3f::axisY;

    };


    /** Flags to define which part of the camera was changed */
    enum ECameraDirtyFlags
    {
        CDF_Transform = SHIFT(0),
        CDF_Projection = SHIFT(1),
        CDF_Everything = SHIFT(2)
    };


    /** Projection type matrix (for renderer) */
    enum EProjectionType : uint8
    {
        PT_Perspective,
        PT_Orthographic
    };


    /** Base camera data, contains all the camera settings */
    class ENGINE_API CameraBase
    {
    public:

        /** Set orthographic projection settings [cause main and render thread sync] */
        void setProjectionSettings(const OrthographicSettings& settings)
        {

            camera_markDirty(ECameraDirtyFlags::CDF_Projection);
            if (isOrthographic()) camera_updateFrustum();
        }

        /** Set perspective projection settings [cause main and render thread sync] */
        void setProjectionSettings(const PerspectiveSettings& settings)
        {

            camera_markDirty(ECameraDirtyFlags::CDF_Projection);
            if (isPerspective()) camera_updateFrustum();
        }

        /** Set camera world transformation [cause main and render thread sync] */
        void setCameraSpace(const CameraSpace& space)
        {

            camera_markDirty(ECameraDirtyFlags::CDF_Transform);
            camera_updateFrustum();
            camera_updateView();
        }

        /** @return True if uses perspective projection */
        bool isPerspective() const { return mProjectionType == PT_Perspective; }

        /** @return True if uses ortho projection */
        bool isOrthographic() const { return mProjectionType == PT_Orthographic; }

        /** @return Projection type */
        EProjectionType getProjectionType() const { return mProjectionType; }

        /** @return Projection sittings */
        const PerspectiveSettings& getPerspectiveSettings() const { return mPerspective; }

        /** @return Projection sittings */
        const OrthographicSettings& getOrthographicSettings() const { return mOrtho; }

        /** @return World space transformation */
        const CameraSpace& geWorldSpace() const { return mWorldSpace; }

        /** @return View matrix */
        const Mat4x4f& getView() const { return mView; }

        /** @return View frustum for culling */
        const Frustum& getFrustum() const { return mFrustum; }

    protected:

        /** Mark dirty for future synchronization with render thread */
        virtual void camera_markDirty(ECameraDirtyFlags flags) {};

        /** Updates camera view */
        virtual void camera_updateView();

        /** Updates camera frustum */
        virtual void camera_updateFrustum();

    protected:

        /** Type of used projection matrix */
        EProjectionType mProjectionType;

        /** Perspective camera setup params */
        OrthographicSettings mOrtho;

        /** Orthographic camera setup params */
        PerspectiveSettings mPerspective;

        /** Camera world space position/orientation/direction */
        CameraSpace mWorldSpace;

        /** View matrix of this camera (reversed transformations) */
        Mat4x4f mView;

        /** View frustum (re-calculate if become dirty) */
        Frustum mFrustum;

    };

} // namespace Berserk

#endif //BERSERK_CAMERABASE_H