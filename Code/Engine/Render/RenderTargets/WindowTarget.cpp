/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RenderTargets/WindowTarget.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Render {

        WindowTarget::WindowTarget(const TPtrShared<Window> &window) {
            BERSERK_COND_ERROR_RET(window.isNotNull(), "Passed null window handle");
            BERSERK_COND_ERROR_RET(window->getState() != EWindowState::Closed, "Window already closed");

            mWindow = window;
            mWindowSize = mWindow->getSize();

            mHasColor = true;
            mHasDepth = true;
            mHasStencil = true;
        }
        
        void WindowTarget::bind(class RHIDrawList &drawList) {
            auto& hnd = mWindow;
            auto viewport = Region2i{0,0,mWindowSize.x(),mWindowSize.y()};

            drawList.bindWindow(hnd, viewport, mClearColor, mClearDepth, mClearStencil);
        }

        void WindowTarget::extractFormat(struct RHIFramebufferFormatDesc &formatDesc) const {
            auto& device = RHIDevice::getSingleton();

            formatDesc.useDepthStencil = true;
            formatDesc.colorFormats.resize(1);
            // May not specify, since it is used for window rendering
        }

        void WindowTarget::extractDefaultBlendState(class RHIBlendStateDesc &blendStateDesc) const {
            blendStateDesc.attachments.resize(1);
            blendStateDesc.attachments[0].enable = false;
        }
    }
}