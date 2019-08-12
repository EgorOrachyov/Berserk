//
// Created by Egor Orachyov on 2019-08-12.
//

#ifndef BERSERK_RENDERSCENEBASE_H
#define BERSERK_RENDERSCENEBASE_H

namespace Berserk
{

    /**
     * Base class for any render scene object info.
     * This info is used only by render system.
     * Contains object state info and must be set/get only by render thread.
     *
     * Also reference to this info must be stored
     * in the render scene class for traverse access
     * and render queue filling state.
     */
    class RenderSceneInfo
    {
    public:

        virtual ~RenderSceneInfo() = default;
    };

}

#endif //BERSERK_RENDERSCENEBASE_H
