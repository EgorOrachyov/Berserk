//
// Created by Egor Orachyov on 2019-08-15.
//

#ifndef BERSERK_ENGINEUTILS_H
#define BERSERK_ENGINEUTILS_H

#include <Engine/EngineResources.h>
#include <Object/ObjectHandle.h>

namespace Berserk
{

    class ENGINE_API EngineUtils final
    {
    public:

        template <class T>
        static ResourceHandle<T> createResHandle(const TSharedPtr<T> res, IAllocator &allocator = Allocator::get())
        {
            auto data = allocator.mem_new<ResourceHandleData>((TSharedPtr<Resource>) res);
            TSharedPtr<ResourceHandleData> resData(data, &allocator);
            return ResourceHandle<T>(resData);
        }

        template <class T>
        static ObjectHandle<T> createObjHandle(const TSharedPtr<T> res, IAllocator &allocator = Allocator::get())
        {
            auto data = allocator.mem_new<ObjectHandleData>((TSharedPtr<Object>) res);
            TSharedPtr<ObjectHandleData> resData(data, &allocator);
            return ObjectHandle<T>(resData);
        }

        template <typename T>
        static TSharedPtr<T> createPtr(T& object, IAllocator* allocator = nullptr)
        {
            return TSharedPtr<T>(&object, allocator);
        }

    };

} // namespace Berserk

#endif //BERSERK_ENGINEUTILS_H