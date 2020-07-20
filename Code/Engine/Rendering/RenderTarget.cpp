/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/RenderTarget.h>

namespace Berserk {
    namespace Rendering {

        void RenderTarget::addResizeListener(RenderTargetResizeListener &listener) {
            auto contains = mResizeListeners.contains(&listener);
            BERSERK_COND_ERROR_RET(!contains, "An attempt to re-subscribe resize listener");
            mResizeListeners.add(&listener);
        }

        void RenderTarget::removeResizeListener(RenderTargetResizeListener &listener) {
            mResizeListeners.removeElement(&listener);
        }

        float RenderTarget::getAreaAspect() const {
            return (mRenderingArea.getH() > 0 ? (float) mRenderingArea.getW() / (float) mRenderingArea.getH() : 0.0f);
        }


    }
}
