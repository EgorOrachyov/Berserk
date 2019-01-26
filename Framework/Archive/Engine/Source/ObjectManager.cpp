//
// Created by Egor Orachyov on 26.07.2018.
//

#include "Managers/ObjectManager.h"
#include "HashFunctions/CRC32.h"
#include "Memory/MemoryAllocators.h"
#include "Misc/Delete.h"

namespace Berserk
{

    ObjectManager::ObjectManager()
    {
        mSize = 0;
        mRange = 0;
        mLists = NULL;
    }

    ObjectManager::~ObjectManager()
    {
        destroy();
    }

    void ObjectManager::init(UINT32 range)
    {
        if (range == 0)
        {
            ERROR("Cannot create object manager with 0 size");
            exit(EXIT_FAILURE);
        }

        mSize = 0;
        mRange = range;

        mLists = (SharedList<Object*>*)mem_calloc(range, sizeof(SharedList<Object*>));
        mPool.init((UINT16)mLists[0].getSizeOfNode(), 64);

        for(UINT32 i = 0; i < mRange; i++)
        {
            mLists[i].init(&mPool);
        }
    }

    void ObjectManager::destroy()
    {
        if (mLists != NULL)
        {
            for(UINT32 i = 0; i < mRange; i++)
            {
                mLists[i].empty();
                mLists[i].~SharedList();
            }

            mem_free(mLists);
            mPool.reset();
        }

        mSize = 0;
        mRange = 0;
        mLists = NULL;
    }

    void ObjectManager::add(Object* object)
    {
        if (object == NULL)
        {
            WARNING("An attempt to add NULL object in manager");
            return;
        }

        UINT32 hash = hashCRC32(object->getName().getChars(), object->getName().getSize()) % mRange;

        SharedList<Object*> &current = mLists[hash];

        current.iterate(true);
        while (current.iterate())
        {
            if (current.getCurrent()->getName() == object->getName())
            {
                WARNING("Object replacement with name %s", object->getName().getChars());

                if (!current.getCurrent()->isStatic())
                {
                    SAFE_DELETE(current.getCurrent());
                }

                current.getCurrent() = object;
                return;
            }
        }

        current.add(object);
    }

    void ObjectManager::empty()
    {
        for(UINT32 i = 0; i < mRange; i++)
        {
            mLists[i].empty();
        }
    }

    Object* ObjectManager::get(const CStaticString &name) const
    {
        UINT32 hash = hashCRC32(name.getChars(), name.getSize()) % mRange;

        SharedList<Object*> &current = mLists[hash];

        current.iterate(true);
        while (current.iterate())
        {
            if (current.getCurrent()->getName() == name)
            {
                return current.getCurrent();
            }
        }

        WARNING("Object with name %s not found", name.getChars());
        return NULL;
    }

    UINT32 ObjectManager::getSize() const
    {
        return mSize;
    }

    UINT32 ObjectManager::getRange() const
    {
        return mRange;
    }

    UINT32 ObjectManager::getCapacity() const
    {
        return mPool.getCapacity();
    }

} // namespace Berserk