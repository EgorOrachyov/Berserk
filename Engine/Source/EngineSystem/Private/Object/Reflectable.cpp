//
// Created by Egor Orachyov on 2019-08-06.
//

#include <Object/Reflectable.h>

namespace Berserk
{

    uint32 ReflectableIDs::getNextTypeID()
    {
        return TYPE_ID_COUNTER++;
    }

    uint32 ReflectableIDs::TYPE_ID_COUNTER = 0;

}