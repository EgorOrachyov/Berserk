/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RENDERUPDATE_H
#define BERSERK_RENDERUPDATE_H

namespace Berserk {
    namespace Render {

        enum class ERenderStage {

        };

        /** Allows to subscribe to render module update, which happens on the render module side */
        class RenderUpdateListener {
        public:
            /**
             * Called on specified render stage update
             * @param stage Passed as argument concrete stage
             */
            virtual void onRenderUpdate(ERenderStage stage) = 0;
        };

        /** Manager callback based update cycle on the render module side */
        class RenderUpdate {

        };

    }
}




#endif //BERSERK_RENDERUPDATE_H
