//
// Created by Egor Orachyov on 13.02.2019.
//

#ifndef BERSERK_IRESOURCE_H
#define BERSERK_IRESOURCE_H

#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"
#include "Misc/NewDelete.h"

namespace Berserk
{

    /**
     * Interface for all CPU and GPU resources, which could be created
     * in the time of engine execution. This class, its inheritors
     * and associated resource managers is the part of the whole
     * engine resource management system.
     *
     * Note: that each resource requires explicit initialization and realese
     * methods call, so, only its manager can perform this. Therefore, direct usage
     * of resource by users is prohibited.
     */
    class ENGINE_API IResource
    {
    public:

        virtual ~IResource() = default;

        /** Initializes empty resource instance with chosen name */
        virtual void initialize(const char* name) { /* todo: remove it */ }

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