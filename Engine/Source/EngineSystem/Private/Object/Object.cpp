//
// Created by Egor Orachyov on 2019-07-31.
//

#include "Object/Object.h"
#include <Logging/DebugLogMacros.h>

namespace Berserk
{
    REFLECTABLE_OBJECT_INIT(Object);

    Object::Object(const Berserk::ObjectInitializer &initializer)
        : mEnvironment(&initializer.getEnvironment())
    {

    }

    void Object::markDirty(Berserk::uint32 flags)
    {
        uint32 wasDirty = getDirtyFlags();
        mDirtyFlags |= flags;

        // todo: add object manager communication
        // if (!wasDirty) getObjectManager().notifyObjectDirty(this);
        if (!wasDirty) DEBUG_LOG_DISPLAY("Object: %s [class: %s] marked dirty", getObjectName().get(), getType().get());
    }

} // namespace Berserk