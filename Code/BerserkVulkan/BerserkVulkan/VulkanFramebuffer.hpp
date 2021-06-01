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

#ifndef BERSERK_VULKANFRAMEBUFFER_HPP
#define BERSERK_VULKANFRAMEBUFFER_HPP

#include <BerserkRHI/RHIFramebuffer.hpp>
#include <BerserkVulkan/VulkanDefs.hpp>
#include <BerserkVulkan/VulkanResource.hpp>

namespace Berserk {
    namespace RHI {

        /** Simple container for attachments, actual vulkan object is managed by fbo cache */
        class VulkanFramebuffer: public Framebuffer, public VulkanResource {
        public:
            explicit VulkanFramebuffer(class VulkanDevice& device, const Desc& desc);
            ~VulkanFramebuffer() override;

            void Initialize();
            void Initialize2d();

            /** Retrieves views of internal attachments */
            void GetViews(ArrayFixed<VkImageView, Limits::MAX_COLOR_ATTACHMENTS> &colorAttachments, VkImageView& depthStencil) const;
            /** Retrieves default layouts of internal attachments */
            void GetLayouts(ArrayFixed<VkImageLayout, Limits::MAX_COLOR_ATTACHMENTS> &colorAttachments, VkImageLayout& depthStencil) const;
            /** Retrieves internal storage formats of internal attachments */
            void GetFormats(ArrayFixed<VkFormat, Limits::MAX_COLOR_ATTACHMENTS> &colorAttachments, VkFormat& depthStencil) const;

            bool HasColorBuffers() const { return mDesc.colorTargets.IsNotEmpty(); }
            bool HasDepthBuffer() const { return mHasDepthBuffer; }
            bool HasStencilBuffer() const { return mHasStencilBuffer; }

        private:
            ArrayFixed<VkImageView, Limits::MAX_COLOR_ATTACHMENTS> mColorAttachments;
            VkImageView mDepthStencilAttachment = nullptr;

            bool mHasDepthBuffer = false;
            bool mHasStencilBuffer = false;

            class VulkanDevice& mDevice;
        };

    }
}

#endif //BERSERK_VULKANFRAMEBUFFER_HPP