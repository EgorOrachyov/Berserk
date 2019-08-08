//
// Created by Egor Orachyov on 2019-07-31.
//

#include "Object/Object.h"

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
#if DEBUG
        if (!wasDirty) OutputDevice::printf("Object: %s [class: %s] marked dirty \n", getObjectName().get(), getType().get());
#endif
    }

} // namespace Berserk