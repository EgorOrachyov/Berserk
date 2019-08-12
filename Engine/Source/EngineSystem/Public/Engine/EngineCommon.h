//
// Created by Egor Orachyov on 2019-08-06.
//

#ifndef BERSERK_ENGINECOMMON_H
#define BERSERK_ENGINECOMMON_H

#include <Misc/Types.h>
#include <Misc/Bits.h>
#include <Misc/UsageDescriptors.h>
#include <Math/MathInclude.h>

namespace Berserk
{

    /** Perspective view setup params */
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


    /** Orthographic view setup params */
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

} // namespace Berserk

#endif //BERSERK_ENGINECOMMON_H
