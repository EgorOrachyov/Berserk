//
// Created by Egor Orachyov on 2019-08-14.
//

#ifndef BERSERK_RENDERLAYER_H
#define BERSERK_RENDERLAYER_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk
{

    /** Defines rendering layer order */
    enum ERenderLayer : uint8
    {

        /**
         * Geometry, rendered in the background firstly.
         * After that the depth buffer is cleared and all the other items
         * will appear on to of the background layer
         */
        RL_Background,

        /**
         * Default pass for solid material geometry
         */
        RL_Default,

        /**
         * Transparent elements, rendered in the back to front order
         * with enabled alpha blending
         */
        RL_Transparent,

        /**
         * All the geometry rendered on top of the previously rendered elements
         */
        RL_Foreground,

        /**
         * Foreground geometry, appears only after all previous render pass done
         */
        RL_Interface

    };

}

#endif //BERSERK_RENDERLAYER_H