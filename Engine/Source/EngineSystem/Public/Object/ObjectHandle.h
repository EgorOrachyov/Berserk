//
// Created by Egor Orachyov on 2019-08-07.
//

#ifndef BERSERK_OBJECTHANDLE_H
#define BERSERK_OBJECTHANDLE_H

#include <Pointer/TSharedPtr.h>
#include <Object/Object.h>
#include <Misc/Allocatable.h>

namespace Berserk
{

#ifndef PROFILE_OBJECT_HANDLE
#   define PROFILE_OBJECT_HANDLE
#endif

    /** Data shared between object handles */
    struct ENGINE_API ObjectHandleData final
    {
        ObjectHandleData() = default;

        explicit ObjectHandleData(TSharedPtr<Object> object)
            : mObject(std::move(object))
        {

        }

        ~ObjectHandleData()
        {
#ifdef PROFILE_RESOURCE_HANDLE
            if (mObject.isNull()) OutputDevice::printf("ObjectHandle: Unload object\n");
            else OutputDevice::printf("ObjectHandle: Unload object: [name: %s]\n", mObject->getObjectName().get());
#endif
        }

        TSharedPtr<Object> mObject;

    };

    /**
     * Handles various engine game objects. Acts like a smart pointer with additional
     * help and misc features.
     *
     * Because of game objects could be unloaded in time of the execution, other game objects
     * could have invalid links. Therefore, objects reference to each other via game object handles.
     * In time of main engine processing handles are stay immutable. Only after main loop pass,
     * object manager unloads objects, and its handlers and clients will know about that.
     *
     * @note Each handler must be used only by one thread
     * @tparam T Type of the handled object
     */
    template <class T>
    class ENGINE_API ObjectHandle final : public Allocatable
    {
    public:

        ObjectHandle() = default;

        /** Creates object handle with specified handle data */
        explicit ObjectHandle(TSharedPtr<ObjectHandleData> data)
                : mData(std::move(data))
        {

        }

        ObjectHandle(const ObjectHandle& other) = default;

        ObjectHandle(ObjectHandle&& other) noexcept = default;

        ~ObjectHandle() = default;

        /**
         * Checks, whether this handle handles not null object handle data
         * @return True, if internal handle data pointer not null
         */
        bool isPresent() const
        {
            return !mData.isNull();
        }

        /** @return Pointer to stored object or null otherwise */
        T* operator->() const
        {
            return (T*) mData->mObject.pointer();
        }

        /** @return Pointer to stored resource or null otherwise */
        T* operator->()
        {
            return (T*) mData->mObject.pointer();
        }

        /** @return Pointer to stored object or null otherwise */
        T* pointer() const
        {
            return (T*) mData->mObject.pointer();
        }

        /**
         * @return Return object reference, if handle
         * stores valid and loaded object pointer
         */
        T& get() const
        {
            return *((T*) mData->mObject.pointer());
        }

        /** @return Internal shared object pointer */
        TSharedPtr<Object> getInternalPtr()
        {
            return mData->mObject;
        }

        /**
         * Release an internal pointer to handled object data
         * (Handle will reference no data)
         */
        void release()
        {
            mData = TSharedPtr<ObjectHandleData>();
        }

        template <class TCast>
        ObjectHandle<TCast> cast() const
        {
            return ObjectHandle<TCast>(mData);
        }

        template <class TCast>
        explicit operator ObjectHandle<TCast>() const
        {
            return ObjectHandle<TCast>(mData);
        }

    private:

        /** Data, shared among object handles of concrete object instance. */
        TSharedPtr<ObjectHandleData> mData;

    };

} // namespace Berserk

#endif //BERSERK_OBJECTHANDLE_H
