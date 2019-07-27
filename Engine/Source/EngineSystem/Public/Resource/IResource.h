//
// Created by Egor Orachyov on 2019-07-20.
//

#ifndef BERSERK_IRESOURCE_H
#define BERSERK_IRESOURCE_H

#include <Strings/String.h>

namespace Berserk
{

    /** Base class for all engine resources [primary loadable for IO]. */
    class ENGINE_API IResource
    {
    public:

        /** Support virtual destruction in shared pointers */
        virtual ~IResource() = default;

        /** @return Used memory (bytes) on cpu (RAM) side */
        virtual uint32 getMemoryUsage() const { return 0; };

        /** @return Used memory (bytes) on gpu (VRAM) side */
        virtual uint32 getMemoryUsage_GPU() const { return 0; };

        /** @return Resource name (for debug purposes) */
        virtual const String& getName() const { return String(); };

    };

} // namespace Berserk

#endif //BERSERK_IRESOURCE_H