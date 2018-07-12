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

        virtual ~RenderSystem() = default;

        virtual const CString& getRenderName() const = 0;

        virtual const CString& getShadingLanguageName() const = 0;

    };

} // namespace Berserk

#endif //BERSERK_RENDERSYSTEM_H
