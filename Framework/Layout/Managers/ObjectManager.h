//
// Created by Egor Orachyov on 26.07.2018.
//

#ifndef BERSERKENGINE_OBJECTMANAGER_H
#define BERSERKENGINE_OBJECTMANAGER_H

#include "Objects/Object.h"
#include "Containers/SharedList.h"

namespace Berserk
{

    class ObjectManager
    {
    public:

        ObjectManager();
        ~ObjectManager();

        void init(UINT32 range = 128);
        void destroy();

        void add(Object* object);
        void empty();
        Object* get(const CStaticString &name) const;

        UINT32 getSize() const;
        UINT32 getRange() const;
        UINT32 getCapacity() const;

    private:

        UINT32 mSize;
        UINT32 mRange;
        PoolAllocator mPool;
        SharedList<Object*> *mLists;
    };

} // namespace Berserk

#endif //BERSERKENGINE_OBJECTMANAGER_H