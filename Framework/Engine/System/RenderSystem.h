//
// Created by Egor Orachyov on 09.07.2018.
//

#ifndef BERSERK_RENDERSYSTEM_H
#define BERSERK_RENDERSYSTEM_H

#include "System.h"

namespace Berserk
{

    class RenderSystem : public System
    {
    public:

        virtual const CHAR* getRenderName() const = 0;

    };

} // namespace Berserk

#endif //BERSERK_RENDERSYSTEM_H
