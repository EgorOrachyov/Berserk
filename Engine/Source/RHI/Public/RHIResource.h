//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_RHIRESOURCE_H
#define BERSERK_RHIRESOURCE_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Strings/String.h>

namespace Berserk
{

    /** Base class for all RHI resources */
    class GRAPHICS_API RHIResource
    {
    public:

        /** Support virtual destruction in shared pointers */
        virtual ~RHIResource() = default;

        /** @return Used memory (bytes) on cpu (RAM) side */
        virtual uint32 getMemoryUsage() { return 0; };

        /** @return Used memory (bytes) on gpu (VRAM) side */
        virtual uint32 getMemoryUsage_GPU() { return 0; };

    };

} // namespace Berserk

#endif //BERSERK_RHIRESOURCE_H