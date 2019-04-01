//
// Created by Egor Orachyov on 01.04.2019.
//

#include "Foundation/IObjectInitializer.h"
#include <Misc/Assert.h>

namespace Berserk::EntitySystem
{

    IObjectInitializer::IObjectInitializer(const char *name)
            : mObjectName(name),
              mGenPurposeAllocator(&Allocator::getSingleton())
    {
        FAIL(name, "Null pointer name for object initializer");
    }

} // namespace Berserk::EntitySystem