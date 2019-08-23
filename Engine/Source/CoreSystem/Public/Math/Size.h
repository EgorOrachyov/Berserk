//
// Created by Egor Orachyov on 2019-08-23.
//

#ifndef BERSERK_SIZE_H
#define BERSERK_SIZE_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk
{

    struct CORE_EXPORT Size
    {

        Size() = default;

        Size(uint32 width, uint32 height) : width(width), height(height) {}

        /** Width offset for object */
        uint32 width = 0;

        /** Height offset for object */
        uint32 height = 0;

    };

} // namespace Berserk

#endif //BERSERK_SIZE_H
