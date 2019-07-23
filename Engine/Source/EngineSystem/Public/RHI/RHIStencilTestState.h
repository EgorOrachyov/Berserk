//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_RHISTENCILTESTSTATE_H
#define BERSERK_RHISTENCILTESTSTATE_H

#include <RHI/RHIResource.h>
#include <Rendering/Definitions.h>

namespace Berserk
{

    class GRAPHICS_API RHIStencilTestState : public RHIResource
    {
    public:

        virtual ~RHIStencilTestState() = default;

        /** Enables this state as active */
        virtual void enable() = 0;

    };

    typedef TSharedPtr<RHIStencilTestState> RHIStencilTestStateRef;

} // namespace Berserk

#endif //BERSERK_RHISTENCILTESTSTATE_H