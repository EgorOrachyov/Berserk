/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/IRenderTarget.h>

namespace Berserk {

    void IRenderTarget::addResizeListener(IRenderTargetResizeListener &listener) {
        auto contains = mResizeListeners.contains(&listener);
        BERSERK_COND_ERROR_RET(!contains, "An attempt to re-subscribe resize listener");
        mResizeListeners.add(&listener);
    }

    void IRenderTarget::removeResizeListener(IRenderTargetResizeListener &listener) {
        mResizeListeners.removeElement(&listener);
    }

    float IRenderTarget::getAreaAspect() const {
        return (mRenderingArea.getH() > 0? (float)mRenderingArea.getW()/(float)mRenderingArea.getH(): 0.0f);
    }


}
