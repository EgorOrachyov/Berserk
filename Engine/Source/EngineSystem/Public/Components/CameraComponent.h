//
// Created by Egor Orachyov on 2019-08-06.
//

#ifndef BERSERK_CAMERACOMPONENT_H
#define BERSERK_CAMERACOMPONENT_H

#include <Components/Component.h>
#include <Engine/EngineCommon.h>
#include <Rendering/RenderSceneInfo.h>

namespace Berserk
{

    /** Flags to define which part of the camera was changed */
    enum ECameraDirtyFlags
    {
        CDF_Transform = SHIFT(0),
        CDF_Projection = SHIFT(1),
        CDF_Settings = SHIFT(2),
        CDF_Everything = SHIFT(3)
    };


    /** Projection type matrix (for renderer) */
    enum EProjectionType : uint8
    {
        PT_Perspective,
        PT_Orthographic
    };

    /** Base class for any engine camera, which defines rendering view settings */
    class ENGINE_API CameraComponent : public Component
    {
    public:

        REFLECTABLE_OBJECT(CameraComponent);

    public:

#if DEBUG
        CameraComponent() : Component() { }
#endif

        ~CameraComponent() override = default;

        /** Set auto viewport active [cause main and render thread sync] */
        void setAutoVieport(bool autoViewport)
        {
            if (mUseAutoViewport != autoViewport) camera_markDirty(ECameraDirtyFlags::CDF_Settings);
            mUseAutoViewport = autoViewport;
        }

        /** Set this camera active (enables view of the scene) [cause main and render thread sync] */
        void setIsActive(bool active)
        {
            if (mIsActive != active) camera_markDirty(ECameraDirtyFlags::CDF_Settings);
            mIsActive = active;
        }

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

        /** @return True, if this camera uses auto viewport */
        bool useAutoViewport() const { return mUseAutoViewport; }

        /** @return True if this camera used for rendering on the scene */
        bool isActive() const { return mIsActive; }

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

        /** @return Scene info of this object [for render thread] */
        const TSharedPtr<CameraSceneInfo> &getSceneInfo() const { return mSceneInfo; }

        /** @return Creates render scene info of this object [for render thread] */
        const TSharedPtr<CameraSceneInfo> &createSceneInfo();

    protected:

        /** Mark dirty for future synchronization with render thread */
        virtual void camera_markDirty(uint32 flags)
        {
            markDirty(flags);
        };

        /** Updates camera view */
        virtual void camera_updateView();

        /** Updates camera frustum */
        virtual void camera_updateFrustum();

    protected:

        /** Will render with auto viewport settings */
        bool mUseAutoViewport = true;

        /** Whether this object is active in the scene */
        bool mIsActive = true;

        /** Type of used projection matrix */
        EProjectionType mProjectionType = PT_Perspective;

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

        /** Scene info of this camera on render thread */
        TSharedPtr<CameraSceneInfo> mSceneInfo;

    };

} // namespace Berserk

#endif //BERSERK_CAMERACOMPONENT_H