/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_IRENDERUPDATE_H
#define BERSERK_IRENDERUPDATE_H

#include <Math/Region2i.h>

namespace Berserk {
    namespace Rendering {

        /** Render module update listener */
        class IRenderModuleUpdateListener {
        public:
            /** Called once on pre-update stage of the rendering module */
            virtual void onPreUpdate() = 0;

            /** Called once on post-update stage of the rendering module */
            virtual void onPostUpdate() = 0;
        };

    }
}

#endif //BERSERK_IRENDERUPDATE_H
