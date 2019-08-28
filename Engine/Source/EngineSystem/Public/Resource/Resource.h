//
// Created by Egor Orachyov on 2019-07-20.
//

#ifndef BERSERK_RESOURCE_H
#define BERSERK_RESOURCE_H

#include <Strings/String.h>
#include <Object/UUID.h>
#include <Misc/Allocatable.h>
#include <Resource/TSharedPtr.h>

namespace Berserk
{

    /**
     * Base class for all engine resources [primary loadable from IO].
     *
     * All the engine resources is supposed to be loaded from HHD via
     * OS files, archives and etc. Therefore all the resource objects
     * must provide and support unified resource loading scheme.
     *
     * @note All the resources in the engine loaded via resource
     *       manager, implemented especially for single kind of resource.
     *
     * @note All the resources in the engine could be referenced only
     *       via resource handle, which provides additional features
     *       for async load and hot-reload of resources.
     *
     * @note All the engine resources could be shared among several
     *       threads (main and pool), therefore its data must stay immutable
     *       in time of execution (except loading, reloading)
     */
    class ENGINE_API Resource : public Allocatable
    {
    public:

        // todo: support unified construction model
        // IResource(IResourceManager& manager, IResourceCache& cache,
        // String name, UUID id, bool isReloaded, bool isLoadedAsync,
        // ArchiveReader &reader, ...);

        /** Support virtual destruction in shared pointers */
        virtual ~Resource() = default;

        /**
         * Readable name of the resource [could be not unique]
         * @return Resource name (for debug purposes) *
         */
        virtual const String& getResourceName() const { return mResourceName; };

        /**
         * Unique resource ID among all the engine resources
         * @return Resource UUID
         */
        virtual const UUID& getResourceUUID() const { return mResourceUUID; };

        /**
        * All the used memory by the resource, including its size,
        * in the main RAM memory for CPU. Must include used memory by all
        * the sub-objects of this resource, except other resources.
        *
        * @return Used memory (bytes) on CPU (RAM) side
        */
        virtual uint32 getMemoryUsage() const { return mSizeCPU; };

        /**
         * Used memory by this resource on GPU side in VRAM.
         * Actual only for resources, based of rendering hardware.
         * This value could be approximated by hand.
         *
         * @return Used memory (bytes) on GPU (VRAM) side
         */
        virtual uint32 getMemoryUsage_GPU() const { return mSizeGPU; };

        /**
         * Number of times, when resource state was changed
         * @return Num of state changes of that concrete resource object
         */
        virtual uint32 getStateCache() const { return mStateCache; }

        /**
         * Shows, whether this resource was loaded async in thread pool
         * @return True, if resource was loaded asynchronously
         */
        virtual bool isLoadedAsync() const { return mIsAsyncLoaded; }

        /**
         * Shows, whether this resource is a reloaded version of previously loaded resource
         * @return True, if that resource was reloaded
         */
        virtual bool isReloaded() const { return mIsReloaded; }

        /**
        * Mark explicitly this resource dirty
        * @param flags Specify dirty flags to mark
        */
        virtual void markDirty(uint32 flags) { mDirtyFlags = flags; }

        /**
         * Mark this resource as clear
         * Note: cause no synchronization
         */
        virtual void markClear() { mDirtyFlags = 0; }

        /**
         * Dirty flags for resources, which require synchronization
         * @return Bit integer value with flags
         */
        virtual uint32 getDirtyFlags() const { return mDirtyFlags; }

        /**
         * Valid reference to resource manager of that concrete resource object
         * @return Reference for the resource manager, loaded that resource
         */
        virtual class IResourceManager &gerResourceManager() const { return *mResourceManager; }

    public:

        /** Set resource CPU size */
        void setSizeCPU(uint32 usage) { mSizeCPU = usage; }

        /** Set resource GPU size */
        void setSizeGPU(uint32 usage) { mSizeGPU = usage; }

        /** Set resource name */
        void setName(const String& name) { mResourceName = name; }

        /** Set resource id */
        void setUUID(const UUID& id) { mResourceUUID = id; }

    protected:

        /** Size of this resource in the RAM */
        uint32 mSizeCPU = 0;

        /** Size of this resource in the VRAM */
        uint32 mSizeGPU = 0;

        /** Dirty flags for resources, which require synchronization */
        uint32 mDirtyFlags = 0;

        /** Number of times, when resource state was changed */
        uint32 mStateCache = 0;

        /** True, if that resource was loaded in pool */
        bool mIsAsyncLoaded = false;

        /** True, if this object is reloaded version of previous resource */
        bool mIsReloaded = false;

        /** Resource manager, which managers this resource object */
        class IResourceManager* mResourceManager = nullptr;

        /** Readable name of the resource [could be not unique] */
        String mResourceName;

        /** Unique resource ID among all the engine resources */
        UUID mResourceUUID;

    };

    typedef TSharedPtr<Resource> ResourceRef;

} // namespace Berserk

#endif //BERSERK_RESOURCE_H