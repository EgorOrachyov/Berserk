//
// Created by Egor Orachyov on 09.07.2018.
//

#ifndef BERSERKENGINE_SYSTEM_H
#define BERSERKENGINE_SYSTEM_H

#include "../../Core/Essential/Types.h"

namespace Berserk
{

    class System
    {
    public:

        virtual void init() = 0;

        virtual void destroy() = 0;

        virtual void update() = 0;

        virtual const CHAR* getName() const = 0;

    };

} // namespace Berserk

#endif //BERSERKENGINE_SYSTEM_H