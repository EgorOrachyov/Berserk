//
// Created by Egor Orachyov on 2019-08-03.
//

#ifndef BERSERK_RESOURCEHANDLE_H
#define BERSERK_RESOURCEHANDLE_H

#include <Resource/TSharedPtr.h>

namespace Berserk
{

    /**
     * Data shared between resource handles
     * @tparam T Type of the handled resource
     */
    template <class T>
    struct ENGINE_API ResourceHandleData final
    {
        ResourceHandleData()
            : mIsLoaded(false)
        {

        }

        explicit ResourceHandleData(const TSharedPtr<T>& resource)
            : mIsLoaded(true), mResource(resource)
        {

        }

        volatile bool mIsLoaded;
        volatile TSharedPtr<T> mResource;
    };

    /**
     * Handles various game engine resources. Acts like a smart pointer with additional
     * help and misc features. Provides:
     * 1) access to the loaded resource
     * 2) also allows hot reload of the resource in run-time mode, without updating handler
     * 3) immediately returns handler to async loaded resource, which become valid as soon as resource is loaded
     *
     * @note Each handler must be used only by one thread
     * @tparam T Type of the handler resource
     */
    template <class T>
    class ENGINE_API ResourceHandle final : public Allocatable
    {
    public:

        /** Creates resource handle with specified handle data */
        explicit ResourceHandle(const TSharedPtr<ResourceHandleData<T>> &data)
            : mData(data)
        {

        }

        /**
         * Checks, whether resource was successfully loaded. Useful for async resources, because
         *
         * @return
         */
        bool isLoaded() const
        {
            return mData->mIsLoaded;
        }

         /** @return Pointer to stored resource or null otherwise */
        T* operator->() const
        {
            return mData->mResource.pointer();
        }

        /** @return Pointer to stored resource or null otherwise */
        T* pointer() const
        {
            return mData->mResource.pointer();
        }

        /**
         * @return Return resource reference, if handle
         * stores valid and loaded resource pointer
         */
        T& get() const
        {
            return mData->mResource.get();
        }

        /**
         * Release an internal pointer to handled resource data
         * (Handle will reference no data)
         */
        void release()
        {
            mData = TSharedPtr<HandleData>();
        }

    private:

        /** Data, shared among resource handles of concrete resource instance. */
        typedef ResourceHandleData<T> HandleData;
        TSharedPtr<HandleData> mData;

    };

} // namespace Berserk

#endif //BERSERK_RESOURCEHANDLE_H