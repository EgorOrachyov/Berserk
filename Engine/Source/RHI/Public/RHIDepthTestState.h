//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_RHIDEPTHTESTSTATE_H
#define BERSERK_RHIDEPTHTESTSTATE_H

#include <RHIResource.h>
#include <RHIDefinitions.h>

namespace Berserk
{

    class GRAPHICS_API RHIDepthTestState : public RHIResource
    {
    public:

        virtual ~RHIDepthTestState() = default;

        /** Enables this state as active */
        virtual void enable() = 0;

    };

    typedef TSharedPtr<RHIDepthTestState> RHIDepthTestStateRef;

} // namespace Berserk

#endif //BERSERK_RHIDEPTHTESTSTATE_H