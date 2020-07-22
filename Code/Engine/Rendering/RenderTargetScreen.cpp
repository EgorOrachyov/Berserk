/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RenderTargetScreen.h>
#include <RHI/RHIDescs.h>
#include <LogMacro.h>

namespace Berserk {
    namespace Rendering {

        RenderTargetScreen::RenderTargetScreen(TPtrShared<Window> window, const Color4f &clearColor) {
            mWindow = std::move(window);
            mClearColor = clearColor;

            auto size = mWindow->getSize();
            mTargetSize = size;
            mRenderingArea = Region2i(0, 0, size[0], size[1]);
        }

        void RenderTargetScreen::extractDeclaration(class RHIGraphicsPipelineDesc &desc) const {
            desc.framebufferFormat.colorFormats.resize(1);
            desc.framebufferFormat.colorFormats[0] = EPixelFormat::R8G8B8A8;
            desc.framebufferFormat.useDepthStencil = true;
            desc.blendState.attachments.resize(1);
            desc.blendState.attachments[0].enable = false;
        }

        void RenderTargetScreen::bind(RHIDrawList &drawList) const {
            drawList.bindWindow(mWindow, mRenderingArea, mClearColor, mDepthClearValue, mStencilClearValue);
        }

        void RenderTargetScreen::onPreUpdate() {
            auto size = mWindow->getSize();
            if (size != mTargetSize) {
                BERSERK_LOG_INFO("Resize window render target Format:%s size:(%i,%i)",
                                 EPixelFormatUtil::pixelFormatToString(mPixelFormat), size[0], size[1]);

                // Window was resized - must update size and notify listeners
                auto oldArea = Region2i(0, 0, mTargetSize[0], mTargetSize[1]);
                mTargetSize = size;
                mRenderingArea = Region2i(0, 0, size[0], size[1]);

                for (auto listener: mResizeListeners) {
                    listener->onResized(oldArea, mRenderingArea);
                }
            }
        }

        void RenderTargetScreen::onPostUpdate() {
            // do nothing - do not listen post update
        }

    }
}