//
// Created by Egor Orachyov on 2019-08-15.
//

#ifndef BERSERK_IRENDERSYSTEM_H
#define BERSERK_IRENDERSYSTEM_H

#include <Rendering/IRenderScene.h>

namespace Berserk
{

    /**
     * General engine render system interface. Provides access to
     * engine rendering facilities.
     *
     * @note Allows to create and register 3D space render scenes with
     *       registered objects and view properties to be rendered by render system.
     *
     * @note todo: Provides access to low-level rendering facilities.
     *       Provides functionality to use custom rendering techniques
     *       and create own additional render pass
     *
     * @note Fixed functionality configurable pipeline based render system -
     *       allows use only internal predefined shaders and rendering techniques.
     *       Provides set of base rendering and lightning effects.
     */
    class ENGINE_API IRenderSystem
    {
    public:

        // todo: add info, rendering functionality and config
        // todo: support custom draw pass
        // todo: add overridable materials/shaders

        virtual TSharedPtr<IRenderScene> createScene() = 0;

        virtual void addScene(const TSharedPtr<IRenderScene> &scene) = 0;

        virtual void removeScene(const TSharedPtr<IRenderScene> &scene) = 0;

    };

} // namespace Berserk

#endif //BERSERK_IRENDERSYSTEM_H