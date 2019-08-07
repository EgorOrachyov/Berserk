//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_RHIFACECULLINGSTATE_H
#define BERSERK_RHIFACECULLINGSTATE_H

#include <RHI/RHIResource.h>
#include <Rendering/Definitions.h>

namespace Berserk
{

    class GRAPHICS_API RHIFaceCullingState : public RHIResource
    {
    public:

        ~RHIFaceCullingState() override = default;

        /** Enables this state as active */
        virtual void enable() = 0;

    };

    typedef TSharedPtr<RHIFaceCullingState> RHIFaceCullingStateRef;

} // namespace Berserk

#endif //BERSERK_RHIFACECULLINGSTATE_H