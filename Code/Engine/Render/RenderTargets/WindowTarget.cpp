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

            RHIWindowPassOptions options;
            {
                options.viewport = Region2i(0,0,mWindowSize.x(),mWindowSize.y());
                options.clearMask = { EClearOption::Color, EClearOption::Depth, EClearOption::Stencil };
            }

            drawList.bindWindow(hnd, options);
        }

    }
}