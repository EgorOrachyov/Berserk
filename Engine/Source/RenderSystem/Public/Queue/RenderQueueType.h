//
// Created by Egor Orachyov on 16.04.2019.
//

#ifndef BERSERK_RENDERQUEUETYPE_H
#define BERSERK_RENDERQUEUETYPE_H

namespace Berserk
{

    /**
     * Type of the render queue, which will be used to
     * render this primitive properly
     */
    enum RenderQueueType
    {
        eRQT_BACKGROUND     = 0,    //! Every thing, that should be in the background (Depth buffer cleared after)
        eRQT_GEOMETRY          ,    //! Default (in the main scene field)
        eRQT_ALPHA_BLEND       ,    //! Requires alpha blending and depth sorting
        eRQT_OVERLAY           ,    //! Rendred on top of the result scene (UI, HUD) with additional overlay camera

        eRQT_TOTAL_TYPES
    };

} // namespace Berserk::Render

#endif //BERSERK_RENDERQUEUETYPE_H
