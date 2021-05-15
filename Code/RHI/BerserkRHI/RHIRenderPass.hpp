/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
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