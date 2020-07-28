/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_WINDOWTARGET_H
#define BERSERK_WINDOWTARGET_H

#include <TEnumMask.h>
#include <TPtrShared.h>
#include <Math/Color4.h>
#include <RHI/RHIDescs.h>
#include <RHI/RHIDrawList.h>
#include <Platform/Window.h>
#include <RenderTargets/RenderTarget.h>

namespace Berserk {
    namespace Render {

        /** Rendering system side window target representation */
        class WindowTarget : public RenderTarget {
        public:

            /** Build target for specified window */
            WindowTarget(const TPtrShared<Window> &window);
            ~WindowTarget() override = default;

            void bind(RHIDrawList &drawList) override;

            /** @return Platform window handle */
            const TPtrShared<Window>& getPlatformWindow() const { return mWindow; }

            // todo: remove
            void update() { mWindowSize = mWindow->getSize(); }

        private:

            TPtrShared<Window> mWindow;
            Size2i mWindowSize;
            Color4f mClearColor;
            float mClearDepth = 1.0f;
            uint32 mClearStencil = 0;
            TEnumMask<EClearOption> mClearOptions = { EClearOption::Color, EClearOption::Depth, EClearOption::Stencil };

        };

    }
}

#endif //BERSERK_WINDOWTARGET_H