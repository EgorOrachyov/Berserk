//
// Created by Egor Orachyov on 2019-08-06.
//

#include "Components/CameraComponent.h"

namespace Berserk
{

    REFLECTABLE_OBJECT_INIT(CameraComponent);

    /*

    const TSharedPtr<CameraSceneInfo>& CameraComponent::createSceneInfo()
    {
        // todo: add proper allocation with memory manager

        IAllocator& allocator = Allocator::get();
        auto info = allocator.engine_new_no_args<CameraSceneInfo>();
        info->frustum = getFrustum();
        info->view = getView();
        info->position = geWorldSpace().Position;
        info->direction = geWorldSpace().Direction;
        info->up = geWorldSpace().Up;
        info->useAutoViewport = useAutoViewport();
        info->isActive = isActive();

        mSceneInfo = TSharedPtr<CameraSceneInfo>(info, &allocator);

        return mSceneInfo;
    }

    */

    void CameraComponent::camera_updateView()
    {
        mView = Mat4x4f::lookAt(
                getWorldPosition(),
                getWorldDirection(),
                getWorldUp());
    }

    void CameraComponent::camera_updateFrustum()
    {
        if (isOrthographic())
        {
            mFrustum = Frustum(
                    mPerspective.ViewAngle,
                    mPerspective.ViewAspect,
                    mPerspective.NearViewDistance,
                    mPerspective.FarViewDistance,
                    getWorldPosition(),
                    getWorldDirection(),
                    getWorldUp());
        }
        else if (isPerspective())
        {
            mFrustum = Frustum(
                    mOrtho.ViewNear,
                    mOrtho.ViewFar,
                    mOrtho.ViewBottom,
                    mOrtho.ViewTop,
                    mOrtho.ViewLeft,
                    mOrtho.ViewRight,
                    getWorldPosition(),
                    getWorldDirection(),
                    getWorldUp());
        }
    }

} // namespace Berserk