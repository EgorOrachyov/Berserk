/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/RenderTargetScreen.h>
#include <LogMacro.h>

namespace Berserk {

    RenderTargetScreen::RenderTargetScreen(ISystem::WINDOW_ID windowId, const Color4f &clearColor) {
        mWindowID = windowId;
        mClearColor = clearColor;

        auto size = ISystem::getSingleton().getWindowSize(windowId);
        mTargetSize = size;
        mRenderingArea = Region2i(0,0,size[0],size[1]);
    }

    void RenderTargetScreen::bind(RHIDrawList &drawList) const {
        drawList.bindWindow(mWindowID, mRenderingArea, mClearColor, mDepthClearValue, mStencilClearValue);
    }

    void RenderTargetScreen::onPreUpdate() {
        auto size = ISystem::getSingleton().getWindowSize(mWindowID);
        if (size != mTargetSize) {
            BERSERK_LOG_INFO("Resize window render target Format:%s size:(%i,%i)",
                    EPixelFormatUtil::pixelFormatToString(mPixelFormat), size[0], size[1]);

            // Window was resized - must update size and notify listeners
            auto oldArea = Region2i(0,0,mTargetSize[0],mTargetSize[1]);
            mTargetSize = size;
            mRenderingArea = Region2i(0,0,size[0],size[1]);

            for (auto listener: mResizeListeners) {
                listener->onResized(oldArea, mRenderingArea);
            }
        }
    }

    void RenderTargetScreen::onPostUpdate() {
        // do nothing - do not listen post update
    }

}