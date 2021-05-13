/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHIRENDERPASS_HPP
#define BERSERK_RHIRENDERPASS_HPP

#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkCore/Image/Color.hpp>
#include <BerserkCore/Templates/ArrayFixed.hpp>

namespace Berserk {
    namespace RHI {

        class RenderPass {
        public:

            struct ColorAttachment {
                Color clearColor;
                RenderTargetOption option = RenderTargetOption::ClearStore;
            };

            struct DepthStencilAttachment {
                float depthClear = 1.0f;
                uint32 stencilClear = 0;
                RenderTargetOption option = RenderTargetOption::ClearStore;
            };

            struct Viewport {
                int32 left = 0;
                int32 bottom = 0;
                uint32 width = 0;
                uint32 height = 0;
            };

            ArrayFixed<ColorAttachment, Limits::MAX_COLOR_ATTACHMENTS> colorAttachments;
            DepthStencilAttachment depthStencilAttachment;
            Viewport viewport;

        };

    }
}

#endif //BERSERK_RHIRENDERPASS_HPP