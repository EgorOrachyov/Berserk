//
// Created by Egor Orachyov on 2019-08-28.
//

#ifndef BERSERK_PTRUTILS_H
#define BERSERK_PTRUTILS_H

#include <Resource/TSharedPtr.h>
#include <Resource/TWeakPtr.h>

namespace Berserk
{

    /**
     * Create new shared pointer object by default engine allocator
     * @tparam T Object type
     * @tparam TArgs Constructor arguments types
     * @param args Actual arguments
     * @return Shared pointer of type T
     */
    template <typename T, typename ... TArgs>
    TSharedPtr<T> mem_new_shared(TArgs &&... args)
    {
        IAllocator& alloc = Allocator::get();
        return TSharedPtr<T>(alloc.mem_new<T>(std::forward<TArgs>(args)...), &alloc);
    }

    /**
     * Create new shared pointer object by custom allocator
     * @tparam T Object type
     * @tparam TArgs Constructor arguments types
     * @param alloc Allocator to allocate object
     * @param args Actual arguments
     * @return Shared pointer of type T
     */
    template <typename T, typename ... TArgs>
    TSharedPtr<T> mem_new_shared_alloc(IAllocator &alloc, TArgs &&... args)
    {
        return TSharedPtr<T>(alloc.mem_new<T>(std::forward<TArgs>(args)...), &alloc);
    }

} // namespace Berserk

#endif //BERSERK_PTRUTILS_H
