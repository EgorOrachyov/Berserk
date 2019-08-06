//
// Created by Egor Orachyov on 2019-08-06.
//

#include <Components/CameraBase.h>

namespace Berserk
{

    void CameraBase::camera_updateView()
    {
        mView = Mat4x4f::lookAt(
                mWorldSpace.Position,
                mWorldSpace.Direction,
                mWorldSpace.Up);
    }

    void CameraBase::camera_updateFrustum()
    {
        if (isOrthographic())
        {
            mFrustum = Frustum(
                    mPerspective.ViewAngle,
                    mPerspective.ViewAspect,
                    mPerspective.NearViewDistance,
                    mPerspective.FarViewDistance,
                    mWorldSpace.Position,
                    mWorldSpace.Direction,
                    mWorldSpace.Up);
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
                    mWorldSpace.Position,
                    mWorldSpace.Direction,
                    mWorldSpace.Up);
        }
    }

} // namespace Berserk

