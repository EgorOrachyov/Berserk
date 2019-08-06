//
// Created by Egor Orachyov on 2019-08-06.
//

#include <Object/Reflectable.h>

namespace Berserk
{

    uint32 IReflectable::getNextTypeId()
    {
        return TYPE_ID_COUNTER++;
    }

    uint32 IReflectable::TYPE_ID_COUNTER = 0;

}