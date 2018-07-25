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

        virtual void begin3dRenderUpdate() = 0;
        virtual void end3dRenderUpdate() = 0;
        virtual void begin2dRenderUpdate() = 0;
        virtual void end2dRenderUpdate() = 0;

        virtual const CString& getRenderName() const = 0;
        virtual const CString& getShadingLanguageName() const = 0;

    };

} // namespace Berserk

#endif //BERSERK_RENDERSYSTEM_H
