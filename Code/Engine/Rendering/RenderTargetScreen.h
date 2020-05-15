/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RENDERTARGETSCREEN_H
#define BERSERK_RENDERTARGETSCREEN_H

#include <Containers/TArray.h>
#include <Platform/ISystem.h>
#include <Rendering/IRenderTarget.h>
#include <Rendering/IRenderUpdate.h>

namespace Berserk {
    namespace Rendering {

        /** Wraps Platform system screen as full-screen target */
        class RenderTargetScreen : public IRenderTarget, public IRenderModuleUpdateListener {
        public:
            RenderTargetScreen(ISystem::WINDOW_ID windowId, const Color4f &clearColor = Color4f(0.0f));
            ~RenderTargetScreen() override = default;

            void extractDeclaration(class RHIGraphicsPipelineDesc &desc) const override;
            void bind(RHIDrawList &drawList) const override;

            bool isScreenTarget() const override { return true; }
            bool isTextureTarget() const override { return false; }
            bool isProxyTarget() const override { return false; }

            void onPreUpdate() override;
            void onPostUpdate() override;

            ISystem::WINDOW_ID getWindowID() const { return mWindowID; }

        private:
            ISystem::WINDOW_ID mWindowID;
        };

    }
}

#endif //BERSERK_RENDERTARGETSCREEN_H