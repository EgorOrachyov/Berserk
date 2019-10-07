//
// Created by Egor Orachyov on 2019-08-03.
//

#ifndef BERSERK_RESOURCEHANDLE_H
#define BERSERK_RESOURCEHANDLE_H

#include <Pointer/TSharedPtr.h>
#include <Resource/Resource.h>
#include <Misc/Allocatable.h>
#include <IO/OutputDevice.h>

namespace Berserk
{

#ifndef PROFILE_RESOURCE_HANDLE
#   define PROFILE_RESOURCE_HANDLE
#endif

    /** Data shared between resource handles */
    struct ENGINE_API ResourceHandleData final : public Allocatable
    {
        ResourceHandleData() = default;

        explicit ResourceHandleData(TSharedPtr<Resource> resource)
            : mIsLoaded(true), mResource(std::move(resource))
        {

        }

        ResourceHandleData(bool isLoaded, TSharedPtr<Resource> resource)
                : mIsLoaded(isLoaded), mResource(std::move(resource))
        {

        }

        ~ResourceHandleData()
        {
#ifdef PROFILE_RESOURCE_HANDLE
            if (mResource.isNull()) OutputDevice::printf("ResourceHandle: Unload resource \n");
            else OutputDevice::printf("ResourceHandle: Unload resource [name: %s]\n", mResource->getResourceName().get());
#endif
        }

        volatile bool mIsLoaded = false;
        TSharedPtr<Resource> mResource;

    };

    /**
     * Handles various game engine resources. Acts like a smart pointer with additional
     * help and misc features. Provides:
     * 1) access to the loaded resource
     * 2) also allows hot reload of the resource in run-time mode, without updating handler
     * 3) immediately returns handler to async loaded resource, which become valid as soon as resource is loaded
     *
     * @note Each handler must be used only by one thread
     * @tparam T Type of the handled resource
     */
    template <class T>
    class ENGINE_API ResourceHandle final : public Allocatable
    {
    public:

        ResourceHandle() = default;

        /** Creates resource handle with specified handle data */
        explicit ResourceHandle(TSharedPtr<ResourceHandleData> data)
            : mData(std::move(data))
        {

        }

        /**
         * Checks, whether resource was successfully loaded. Useful for async resources, because
         * @return True if loaded and ready to use
         */
        bool isLoaded() const
        {
            return mData->mIsLoaded;
        }

        /**
         * Checks, whether this handle handles or not null resource handle data
         * @return True, if internal handle data pointer not null
         */
        bool isPresent() const
        {
            return !mData.isNull();
        }

        /** @return Pointer to stored resource or null otherwise */
        T* operator->() const
        {
            return (T*) mData->mResource.pointer();
        }

        /** @return Pointer to stored resource or null otherwise */
        T* operator->()
        {
            return (T*) mData->mResource.pointer();
        }

        /** @return Pointer to stored resource or null otherwise */
        T* pointer() const
        {
            return (T*) mData->mResource.pointer();
        }

        /**
         * @return Return resource reference, if handle
         * stores valid and loaded resource pointer
         */
        T& get() const
        {
            return (T&) mData->mResource.get();
        }

        /** @return Internal shared resource pointer */
        TSharedPtr<Resource> getInternalPtr()
        {
            return mData->mResource;
        }

        /**
         * Release an internal pointer to handled resource data
         * (Handle will reference no data)
         */
        void release()
        {
            mData = TSharedPtr<ResourceHandleData>();
        }

        template <class TCast>
        ResourceHandle<TCast> cast() const
        {
            return ResourceHandle<TCast>(mData);
        }

        template <class TCast>
        explicit operator ResourceHandle<TCast>() const
        {
            return ResourceHandle<TCast>(mData);
        }

    private:

        /** Data, shared among resource handles of concrete resource instance. */
        TSharedPtr<ResourceHandleData> mData;

    };

} // namespace Berserk

#endif //BERSERK_RESOURCEHANDLE_H