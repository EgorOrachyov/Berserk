//
// Created by Egor Orachyov on 25.06.2019.
//

#ifndef BERSERK_SHAREDPTRMANAGER_H
#define BERSERK_SHAREDPTRMANAGER_H

#include <Misc/Types.h>
#include <Memory/PoolAllocator.h>

namespace Berserk
{

    class ENGINE_API SharedPtrManager
    {
    public:

        struct SharedPtrInfo
        {
        public:

        private:

            uint32 mReferenceCount = 0;



        };

    };

} // namespace Berserk

#endif //BERSERK_SHAREDPTRMANAGER_H