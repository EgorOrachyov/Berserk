//
// Created by Egor Orachyov on 13.02.2019.
//

#ifndef BERSERK_IRESOURCE_H
#define BERSERK_IRESOURCE_H

#include "Public/Misc/Types.h"

namespace Berserk
{

    /**
     * Interface for all CPU and GPU resources, which could be created
     * in the time of engine execution. This class, its inheritors
     * and associated resource managers is the part of the whole
     * engine resource management system.
     */
    class IResource
    {
    public:

        virtual ~IResource() = 0;

        /** Initializes empty resource instance */
        virtual void initialize() = 0;

        /** Add reference to resource if (f.e. you copy that) */
        virtual void addReference() = 0;

        /** Try to delete resource if its reference count == 0 */
        virtual void release() = 0;

        /** @return References count to this resource */
        virtual uint32 getReferenceCount() = 0;

        /** @return Memory cost of this resource (on CPU side only) */
        virtual uint32 getMemoryUsage() = 0;

        /** @return String name of resource (its unique identifier) */
        virtual const char* getName() = 0;

    };

} // namespace Berserk

#endif //BERSERK_IRESOURCE_H