/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RenderTarget.h>

namespace Berserk {
    namespace Render {

        void RenderTarget::addResizeListener(RenderTargetResizeListener &listener) {
            auto ptr = &listener;

            BERSERK_COND_ERROR_RET(!mListeners.contains(ptr), "Listener already subscribed");
            mListeners.add(ptr);
        }
        
        void RenderTarget::removeResizeListener(RenderTargetResizeListener &listener) {
            auto ptr = &listener;

            BERSERK_COND_ERROR_RET(mListeners.contains(ptr), "Listener was not subscribed");
            mListeners.removeElement(ptr);
        }
        
        void RenderTarget::onResized(Size2i oldSize, Size2i newSize) {
            for (auto listener: mListeners) {
                listener->onTargetResized(oldSize, newSize);
            }
        }
        
    }
}