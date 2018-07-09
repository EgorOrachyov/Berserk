//
// Created by Egor Orachyov on 04.07.2018.
//

#ifndef BERSERK_GLRENDERSYSTEM_H
#define BERSERK_GLRENDERSYSTEM_H

#include "../Engine/System/RenderSystem.h"

namespace Berserk
{

    class GLRenderSystem : public RenderSystem
    {
    public:

        virtual void init() { printf("Init\n"); }

        virtual void update() {}

        virtual void destroy() {}

        virtual const CHAR* getName() const {}

        virtual const CHAR* getRenderName() const {}

    };

} // namespace Berserk

#endif //BERSERK_GLRENDERSYSTEM_H