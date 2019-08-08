//
// Created by Egor Orachyov on 2019-08-08.
//

#ifndef BERSERK_RHISUPPORT_H
#define BERSERK_RHISUPPORT_H

#include <RHI/RHIResource.h>

namespace Berserk
{

    /** Utils which depend on render hardware */
    class RHISupport : public RHIResource
    {
    public:

        ~RHISupport() override = default;

        /**
         * Perspective projection
         *
         * @note OpenGL and DirectX must provide different matrices
         *
         * @warning Fovy should be me more than 0
         * @warning Aspect should be more than 0
         *
         * @param fovy   Angle between top and bottom sides in radians
         * @param aspect Width-to-height ratio
         * @param near   Near clip plane
         * @param far    Far clip plane
         * @return
         */
        virtual Mat4x4f projectionMatrix(float32 fovy, float32 aspect, float32 near, float32 far) = 0;

    };

    typedef TSharedPtr<RHISupport> RHISupportRef;

} // namespace Berserk

#endif //BERSERK_RHISUPPORT_H