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

#include <BerserkVulkan/VulkanFramebuffer.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanTexture.hpp>
#include <BerserkCore/Image/PixelUtil.hpp>

namespace Berserk {
    namespace RHI {

        VulkanFramebuffer::VulkanFramebuffer(class VulkanDevice& device, const Framebuffer::Desc &desc) : mDevice(device) {
            mDesc = desc;
        }

        VulkanFramebuffer::~VulkanFramebuffer() {
            BERSERK_VK_LOG_INFO(BERSERK_TEXT("Create Framebuffer2d: {0}"), GetName());

            auto device = mDevice.GetDevice();

            for (auto view: mColorAttachments) {
                vkDestroyImageView(device, view, nullptr);
            }

            if (mDepthStencilAttachment)
                vkDestroyImageView(device, mDepthStencilAttachment, nullptr);

            mColorAttachments.Clear();
            mDepthStencilAttachment = VK_NULL_HANDLE;
        }

        void VulkanFramebuffer::Initialize() {
            // Currently only 2d framebuffers are supported
            Initialize2d();
        }

        void VulkanFramebuffer::Initialize2d() {
            uint32 width = mDesc.width;
            uint32 height = mDesc.height;
            auto& colors = mDesc.colorTargets;
            auto& depthStencil = mDesc.depthStencilTarget;

            assert(width > 0);
            assert(height > 0);
            assert(colors.IsNotEmpty() || depthStencil.target.IsNotNull());

            for (auto& attachment: colors) {
                auto target = (VulkanTexture*) attachment.target.Get();
                auto size = PixelUtil::GetMipSize(attachment.mipLevel, target->GetWidth(), target->GetHeight());

                assert(target);
                assert(target->GetTextureType() == TextureType::Texture2d || target->GetTextureType() == TextureType::TextureCube || target->GetTextureType() == TextureType::Texture2dArray);
                assert(target->UsageColorAttachment());
                assert(width == size.x());
                assert(height == size.y());
                assert(attachment.mipLevel < target->GetMipsCount());
            }

            if (depthStencil.target) {
                auto target = (VulkanTexture*) depthStencil.target.Get();
                auto size = PixelUtil::GetMipSize(depthStencil.mipLevel, target->GetWidth(), target->GetHeight());

                assert(target);
                assert(target->GetTextureType() == TextureType::Texture2d || target->GetTextureType() == TextureType::TextureCube || target->GetTextureType() == TextureType::Texture2dArray);
                assert(target->UsageDepthStencilAttachment());
                assert(width == size.x());
                assert(height == size.y());
                assert(depthStencil.mipLevel < target->GetMipsCount());

                auto isSuitable = VulkanDefs::IsSuitableForDepthStencil(target->GetTextureFormat(), mHasDepthBuffer, mHasStencilBuffer);

                assert(isSuitable);
            }

            auto createView = [&](const AttachmentDesc& attachment) {
                auto target = (VulkanTexture*) attachment.target.Get();
                auto format = target->GetTextureFormat();

                VkImageViewCreateInfo viewInfo{};
                viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                viewInfo.image = target->GetImage();
                viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                viewInfo.format = VulkanDefs::GetTextureFormat(format);
                viewInfo.subresourceRange.aspectMask = VulkanDefs::GetAspectFlags(format);
                viewInfo.subresourceRange.baseMipLevel = attachment.mipLevel;
                viewInfo.subresourceRange.levelCount = 1;
                viewInfo.subresourceRange.baseArrayLayer = attachment.arraySlice;
                viewInfo.subresourceRange.layerCount = 1;
                viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

                VkImageView view;

                BERSERK_VK_CHECK(vkCreateImageView(mDevice.GetDevice(), &viewInfo, nullptr, &view));

                return view;
            };

            for (auto& attachment: colors)
                mColorAttachments.Add(createView(attachment));
            if (depthStencil.target)
                mDepthStencilAttachment = createView(depthStencil);

            BERSERK_VK_LOG_INFO(BERSERK_TEXT("Create Framebuffer2d: {0}"), GetName());
        }

        void VulkanFramebuffer::GetViews(ArrayFixed<VkImageView, Limits::MAX_COLOR_ATTACHMENTS> &colorAttachments, VkImageView &depthStencil) const {
            colorAttachments = mColorAttachments;

            if (mHasDepthBuffer || mHasStencilBuffer) {
                depthStencil = mDepthStencilAttachment;
            }
        }

        void VulkanFramebuffer::GetLayouts(ArrayFixed<VkImageLayout, Limits::MAX_COLOR_ATTACHMENTS> &colorAttachments, VkImageLayout &depthStencil) const {
            for (auto& attachment: mDesc.colorTargets) {
                auto texture = (VulkanTexture*) attachment.target.Get();
                colorAttachments.Add(texture->GetPrimaryLayout());
            }

            if (mHasDepthBuffer || mHasStencilBuffer) {
                auto texture = (VulkanTexture*) mDesc.depthStencilTarget.target.Get();
                depthStencil = texture->GetPrimaryLayout();
            }
        }

        void VulkanFramebuffer::GetFormats(ArrayFixed<VkFormat, Limits::MAX_COLOR_ATTACHMENTS> &colorAttachments, VkFormat &depthStencil) const {
            for (auto& attachment: mDesc.colorTargets) {
                auto format = attachment.target->GetTextureFormat();
                colorAttachments.Add(VulkanDefs::GetTextureFormat(format));
            }

            if (mHasDepthBuffer || mHasStencilBuffer) {
                auto format = mDesc.depthStencilTarget.target->GetTextureFormat();
                depthStencil = VulkanDefs::GetTextureFormat(format);
            }
        }

    }
}