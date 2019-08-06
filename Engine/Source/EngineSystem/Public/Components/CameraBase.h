//
// Created by Egor Orachyov on 2019-08-06.
//

#ifndef BERSERK_CAMERABASE_H
#define BERSERK_CAMERABASE_H

#include <Engine/EngineCommon.h>

namespace Berserk
{

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
        void setCameraSpace(const TransformSpace& space)
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
        const TransformSpace& geWorldSpace() const { return mWorldSpace; }

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
        TransformSpace mWorldSpace;

        /** View matrix of this camera (reversed transformations) */
        Mat4x4f mView;

        /** View frustum (re-calculate if become dirty) */
        Frustum mFrustum;

    };

} // namespace Berserk

#endif //BERSERK_CAMERABASE_H