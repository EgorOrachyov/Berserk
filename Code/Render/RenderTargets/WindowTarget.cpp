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
            bind(drawList, mClearOptions);
        }

        void WindowTarget::bind(class RHIDrawList &drawList, const TEnumMask<EClearOption> &clearOptions) {
            bind(drawList, clearOptions, Region2i(0,0,mWindowSize[0],mWindowSize[1]));
        }

        void WindowTarget::bind(class RHIDrawList &drawList, const TEnumMask<EClearOption> &clearOptions, const Region2i &subRegion) {
            auto& hnd = mWindow;

            auto x = Math::clamp(subRegion.getX(), 0, mWindowSize[0]);
            auto y = Math::clamp(subRegion.getY(), 0, mWindowSize[1]);
            auto w = Math::clamp(subRegion.getW(), 0, mWindowSize[0]);
            auto h = Math::clamp(subRegion.getH(), 0, mWindowSize[1]);

            RHIWindowPassOptions options;
            {
                options.viewport = Region2i(x,y,w,h);
                options.clearMask = clearOptions;
                options.clearColor = mClearColor;
                options.clearDepth = mClearDepth;
                options.clearStencil = mClearStencil;
            }

            drawList.bindWindow(hnd, options);
        }

    }
}