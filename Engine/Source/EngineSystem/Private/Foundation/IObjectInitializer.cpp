//
// Created by Egor Orachyov on 01.04.2019.
//

#include "Foundation/IObjectInitializer.h"
#include <Misc/Assert.h>

namespace Berserk::Engine
{

    IObjectInitializer::IObjectInitializer(const char *name, IAllocator* allocator, IAllocator* objects)
            : mObjectName(name)
    {
        FAIL(name, "Null pointer name for object initializer");

        if (allocator) mGenPurposeAllocator = allocator;
        else mGenPurposeAllocator = &Allocator::getSingleton();

        if (objects) mObjectsAllocator = objects;
        else mGenPurposeAllocator = &Allocator::getSingleton();
    }

} // namespace Berserk::EntitySystem