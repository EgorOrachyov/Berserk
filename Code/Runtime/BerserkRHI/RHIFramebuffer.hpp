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

#ifndef BERSERK_RHIFRAMEBUFFER_HPP
#define BERSERK_RHIFRAMEBUFFER_HPP

#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkRHI/RHIResource.hpp>
#include <BerserkRHI/RHITexture.hpp>
#include <BerserkCore/Templates/ArrayFixed.hpp>

namespace Berserk {
    namespace RHI {

        class Framebuffer: public Resource {
        public:

            /** Describes single render target attachment */
            struct AttachmentDesc {
                RefCounted<Texture> target;
                uint32 arraySlice = 0;
                uint32 face = 0;
                uint32 mipLevel = 0;
            };

            struct Desc {
                StringName name;
                uint32 width;
                uint32 height;
                AttachmentDesc depthStencilTarget;
                ArrayFixed<AttachmentDesc, Limits::MAX_COLOR_ATTACHMENTS> colorTargets;
            };

            ~Framebuffer() override = default;

            /** @return Texture width in pixels */
            uint32 GetWidth() const { return mDesc.width; }

            /** @return Texture height in pixels */
            uint32 GetHeight() const { return mDesc.height; }

            /** @return Number of color attachments in this framebuffer */
            uint64 GetColorAttachmentsCount() const { return mDesc.colorTargets.GetSize(); }

            /** @return Framebuffer debug name (displayed in the vk debugger)*/
            const StringName& GetName() const { return mDesc.name; }

            /** @return Render target desc */
            const Desc& GetDesc() const { return mDesc; }

        protected:

            /** Render target desc */
            Desc mDesc;
        };

    }
}

#endif //BERSERK_RHIFRAMEBUFFER_HPP