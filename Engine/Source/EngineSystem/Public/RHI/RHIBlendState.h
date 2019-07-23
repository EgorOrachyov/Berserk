//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_RHIBLENDSTATE_H
#define BERSERK_RHIBLENDSTATE_H

#include <RHI/RHIResource.h>
#include <Rendering/Definitions.h>

namespace Berserk
{

    class GRAPHICS_API RHIBlendState : public RHIResource
    {
    public:

        virtual ~RHIBlendState() = default;

        /** Enables this state as active */
        virtual void enable() = 0;

    };

    typedef TSharedPtr<RHIBlendState> RHIBlendStateRef;

} // namespace Berserk

#endif //BERSERK_RHIBLENDSTATE_H