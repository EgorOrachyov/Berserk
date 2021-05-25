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
#include <BerserkVulkan/VulkanTexture.hpp>
#include <BerserkCore/Image/PixelUtil.hpp>

namespace Berserk {
    namespace RHI {

        VulkanFramebuffer::VulkanFramebuffer(const Framebuffer::Desc &desc) {
            mDesc = desc;
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
        }

        void VulkanFramebuffer::GetViews(ArrayFixed<VkImageView, Limits::MAX_COLOR_ATTACHMENTS> &colorAttachments, VkImageView &depthStencil) const {
            for (auto& attachment: mDesc.colorTargets) {
                auto texture = (VulkanTexture*) attachment.target.Get();
                colorAttachments.Add(texture->GetView());
            }

            if (mHasDepthBuffer || mHasStencilBuffer) {
                auto texture = (VulkanTexture*) mDesc.depthStencilTarget.target.Get();
                depthStencil = texture->GetView();
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