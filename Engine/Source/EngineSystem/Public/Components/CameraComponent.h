//
// Created by Egor Orachyov on 2019-08-06.
//

#ifndef BERSERK_CAMERACOMPONENT_H
#define BERSERK_CAMERACOMPONENT_H

#include <Components/Component.h>
#include <Engine/EngineCommon.h>
#include <Rendering/RenderSceneBase.h>

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
            mUseAutoViewport = autoViewport;
            if (mUseAutoViewport != autoViewport)
                camera_markDirty(ECameraDirtyFlags::CDF_Settings);
        }

        /** Set this camera active (enables view of the scene) [cause main and render thread sync] */
        void setIsActive(bool active)
        {
            mIsActive = active;
            if (mIsActive != active) camera_markDirty(ECameraDirtyFlags::CDF_Settings);
        }

        /** Set orthographic projection settings [cause main and render thread sync] */
        void setProjectionSettings(const OrthographicSettings& settings)
        {
            mOrtho = settings;
            camera_markDirty(ECameraDirtyFlags::CDF_Projection);
        }

        /** Set perspective projection settings [cause main and render thread sync] */
        void setProjectionSettings(const PerspectiveSettings& settings)
        {
            mPerspective = settings;
            camera_markDirty(ECameraDirtyFlags::CDF_Projection);
        }

        /** Set world position [cause main and render thread sync] */
        void setWorldPosition(const Vec3f& position)
        {
            mWorldPosition = position;
            camera_markDirty(ECameraDirtyFlags::CDF_Transform);
        }

        /** Set world rotation [cause main and render thread sync] */
        void setWorldRotation(const Quatf& rotation)
        {
            mWorldRotation = rotation;
            camera_markDirty(ECameraDirtyFlags::CDF_Transform);
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

        /** @return World space position */
        const Vec3f& getWorldPosition() const { return mWorldPosition; }

        /** @return World camera direction vector (Z axis in local space) */
        Vec3f getWorldDirection() const { return mWorldRotation.rotate(Vec3f::axisZ); }

        /** @return World Up camera vector (Y axis in local space) */
        Vec3f getWorldUp() const { return mWorldRotation.rotate(Vec3f::axisY); }

        /** @return World space rotation */
        const Quatf& getWorldRotation() const { return mWorldRotation; }

        /** @return View matrix */
        const Mat4x4f& getView() const { return mView; }

        /** @return View frustum for culling */
        const Frustum& getFrustum() const { return mFrustum; }

        /**
         * @return True whether this object is attached to the render scene
         * @warning Should be used only by render system
         */
        bool attachedToRenderScene() const { return mSceneInfo.isPresent(); }

        /**
         * @return Scene info of this object
         * @warning Should be used only by render system
         */
        const TSharedPtr<RenderSceneInfo> &getSceneInfo() const { return mSceneInfo; }

        /**
         * Set render scene info of this object
         * @warning Should be used only by render system
         */
        void setSceneInfo(const TSharedPtr<RenderSceneInfo> &info) const { mSceneInfo = info; }

        // todo: add on window re-sized callback
        // todo: add render system callbacks

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

        /** World space position */
        Vec3f mWorldPosition = Vec3f(0.0f);

        /** World space rotation */
        Quatf mWorldRotation = Quatf();

        /** View matrix of this camera (reversed transformations) */
        Mat4x4f mView;

        /** View frustum (re-calculate if become dirty) */
        Frustum mFrustum;

        /** Scene info of this camera on render thread */
        mutable TSharedPtr<RenderSceneInfo> mSceneInfo;

    };

} // namespace Berserk

#endif //BERSERK_CAMERACOMPONENT_H