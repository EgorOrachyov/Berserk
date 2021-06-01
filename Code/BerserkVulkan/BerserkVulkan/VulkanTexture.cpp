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

#include <BerserkVulkan/VulkanTexture.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanQueues.hpp>
#include <BerserkVulkan/VulkanDebug.hpp>
#include <BerserkVulkan/VulkanUtils.hpp>
#include <BerserkVulkan/VulkanStagePool.hpp>
#include <BerserkVulkan/VulkanCmdBufferManager.hpp>
#include <BerserkCore/Image/PixelUtil.hpp>

namespace Berserk {
    namespace RHI {

        static const char* GetTextureNameFromType(TextureType type) {
            switch (type) {
                case TextureType::Texture2d:
                    return "Texture2d";
                case TextureType::Texture2dArray:
                    return "Texture2dArray";
                case TextureType::TextureCube:
                    return "TextureCube";
                case TextureType::Texture3d:
                    return "Texture3d";
                default:
                    return "!UnsupportedType!";
            }
        }

        VulkanTexture::VulkanTexture(class VulkanDevice& device, const Desc& desc) : mDevice(device) {
            mDesc = desc;
        }

        VulkanTexture::~VulkanTexture() {
            if (mImage) {
                BERSERK_VK_LOG_INFO(BERSERK_TEXT("Release Texture: {0}"), mImage);

                vkDestroyImageView(mDevice.GetDevice(), mView, nullptr);

                VulkanMemoryManager::ImageAllocation allocation{};
                allocation.image = mImage;
                allocation.allocation = mImageAllocation;

                auto& memMan = *mDevice.GetMemoryManager();
                memMan.DeallocateImage(allocation);

                mImage = nullptr;
                mView = nullptr;
            }
        }

        void VulkanTexture::Initialize() {
            auto usage = mDesc.textureUsage;

            if (usage.Get(TextureUsage::ColorAttachment)) {
                mPrimaryLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                mUsageFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
                assert(!usage.Get(TextureUsage::DepthAttachment));
                assert(!usage.Get(TextureUsage::DepthStencilAttachment));
            }
            if (usage.Get(TextureUsage::DepthStencilAttachment)) {
                mPrimaryLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                mUsageFlags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
                assert(!usage.Get(TextureUsage::ColorAttachment));
                assert(!usage.Get(TextureUsage::DepthAttachment));
            }
            if (usage.Get(TextureUsage::DepthAttachment)) {
                mPrimaryLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
                mUsageFlags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
                assert(!usage.Get(TextureUsage::ColorAttachment));
                assert(!usage.Get(TextureUsage::DepthStencilAttachment));
            }
            if (usage.Get(TextureUsage::Sampling)) {
                mPrimaryLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                mUsageFlags |= VK_IMAGE_USAGE_SAMPLED_BIT;
                assert(!usage.Get(TextureUsage::DepthStencilAttachment));
            }

            mUsageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
            mUsageFlags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

            switch (mDesc.textureType) {
                case TextureType::Texture2d:
                    Validate2d();
                    break;
                case TextureType::Texture2dArray:
                    Validate2dArray();
                    break;
                case TextureType::TextureCube:
                    ValidateCube();
                    break;
                default:
                    BERSERK_VK_LOG_ERROR("Unsupported TextureType");
                    break;
            }

            InitializeInternal();

            BERSERK_VK_LOG_INFO(BERSERK_TEXT("Create {0}: {1}"), GetTextureNameFromType(GetTextureType()), mImage);
        }

        void VulkanTexture::UpdateTexture2D(VkCommandBuffer buffer, uint32 mipLevel, const Math::Rect2u &region, const PixelData &memory) {
            assert(GetTextureType() == TextureType::Texture2d);
            UpdateTexture2DArrayInternal(buffer, 0, mipLevel, region, memory);
        }

        void VulkanTexture::UpdateTexture2DArray(VkCommandBuffer buffer, uint32 arrayIndex, uint32 mipLevel, const Math::Rect2u &region, const PixelData &memory) {
            assert(GetTextureType() == TextureType::Texture2dArray);
            UpdateTexture2DArrayInternal(buffer, arrayIndex, mipLevel, region, memory);
        }

        void VulkanTexture::UpdateTextureCube(VkCommandBuffer buffer, TextureCubemapFace face, uint32 mipLevel, const Math::Rect2u &region, const PixelData &memory) {
            assert(GetTextureType() == TextureType::TextureCube);
            assert(VK_IMAGE_ASPECT_COLOR_BIT == VulkanDefs::GetAspectFlags(GetTextureFormat()));
            assert(mipLevel < GetMipsCount());
            assert(region.x() <= region.z());
            assert(region.y() <= region.w());
            assert(CanUpdate());

            auto mipSize = PixelUtil::GetMipSize(mipLevel, GetWidth(), GetHeight());
            assert(region.z() <= mipSize.x());
            assert(region.w() <= mipSize.y());

            auto& memMan = *mDevice.GetMemoryManager();
            auto vmaAlloc = memMan.GetVmaAllocator();

            const auto* pixels = memory.GetData();
            auto pixelDataSize = memory.GetDataSize();
            assert(VulkanDefs::CanCopyImage(GetTextureFormat(), memory.GetDataType(), memory.GetDataFormat()));

            // 1. Allocate staging buffer to transfer image
            auto staging = mDevice.GetStagePool()->AllocateBuffer(pixelDataSize);

            void* mappedRegion;
            BERSERK_VK_CHECK(vmaMapMemory(vmaAlloc, staging.allocation, &mappedRegion));

            // 2. Copy data into staging buffer
            Memory::Copy(mappedRegion, pixels, pixelDataSize);
            vmaUnmapMemory(vmaAlloc, staging.allocation);

            uint32 faceId = VulkanDefs::GetCubeFaceId(face);
            assert(faceId < Limits::MAX_TEXTURE_CUBE_FACES);

            VkImageSubresourceRange subresource{};
            subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            subresource.baseMipLevel = mipLevel;
            subresource.levelCount = 1;
            subresource.baseArrayLayer = faceId;
            subresource.layerCount = 1;

            int32 xoffset = region.x();
            int32 yoffset = region.y();
            uint32 width = region.z();
            uint32 height = region.w();

            VkBufferImageCopy copyRegion{};
            copyRegion.bufferOffset = 0;
            copyRegion.bufferRowLength = 0;
            copyRegion.bufferImageHeight = 0;
            copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copyRegion.imageSubresource.mipLevel = mipLevel;
            copyRegion.imageSubresource.baseArrayLayer = faceId;
            copyRegion.imageSubresource.layerCount = 1;
            copyRegion.imageOffset = {xoffset, yoffset, 0};
            copyRegion.imageExtent = { width, height, 1 };

            // 3. Copy data into image (transition layout automatically)
            BERSERK_VK_BEGIN_LABEL(buffer, "UpdateTexture2D");
            UpdateSubResource(buffer, staging.buffer, copyRegion, subresource);
            BERSERK_VK_END_LABEL(buffer);
        }

        void VulkanTexture::UpdateSubResource(VkCommandBuffer buffer, VkBuffer staging, const VkBufferImageCopy &copy,
                                              const VkImageSubresourceRange &range) {
            auto& utils = *mDevice.GetUtils();

            // 1. Transition image layout into transfer dst
            VkAccessFlags srcFlags;
            VkPipelineStageFlags srcStages;
            GetSrcBarrierSettings(srcFlags, srcStages);

            utils.BarrierImage(buffer,
                               mImage,
                               srcFlags, VK_ACCESS_TRANSFER_WRITE_BIT,
                               mPrimaryLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               srcStages, VK_PIPELINE_STAGE_TRANSFER_BIT,
                               range);

            // 2. Copy staging buffer data into image
            vkCmdCopyBufferToImage(buffer, staging, mImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);

            VkAccessFlags dstFlags;
            VkPipelineStageFlags dstStages;
            GetDstBarrierSetting(dstFlags, dstStages);

            // 3. Transition layout to default
            utils.BarrierImage(buffer,
                               mImage,
                               VK_ACCESS_TRANSFER_WRITE_BIT, dstFlags,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mPrimaryLayout,
                               VK_PIPELINE_STAGE_TRANSFER_BIT, dstStages,
                               range);
        }

        void VulkanTexture::GenerateMipmaps(VkCommandBuffer buffer) {
            assert(GetTextureType() == TextureType::Texture2d || GetTextureType() == TextureType::Texture2dArray || GetTextureType() == TextureType::TextureCube);
            assert(GetMipsCount() == PixelUtil::GetMaxMipsCount(GetWidth(), GetHeight(), GetDepth()));
            assert(VK_IMAGE_ASPECT_COLOR_BIT == VulkanDefs::GetAspectFlags(GetTextureFormat()));
            assert(GetMipsCount() > 0);

            auto type = GetTextureType();
            auto& utils = *mDevice.GetUtils();

            uint32 sourceMip = 0;
            uint32 firstToBlit = 1;
            uint32 mipsToGen = GetMipsCount() - 1;
            uint32 arrayLayers = type == TextureType::TextureCube? Limits::MAX_TEXTURE_CUBE_FACES: GetArraySlices();

            if (mipsToGen == 0)
                return;

            VkImageSubresourceRange blitSource{};
            blitSource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blitSource.baseMipLevel = sourceMip;
            blitSource.levelCount = 1;
            blitSource.baseArrayLayer = 0;
            blitSource.layerCount = arrayLayers;

            BERSERK_VK_BEGIN_LABEL(buffer, "GenerateMipmaps");

            {
                // 1. Transition layout of the source mip level to copy from

                VkAccessFlags srcFlags;
                VkPipelineStageFlags srcStages;
                GetSrcBarrierSettings(srcFlags, srcStages);

                utils.BarrierImage(buffer,
                                   mImage,
                                   srcFlags, VK_ACCESS_TRANSFER_READ_BIT,
                                   mPrimaryLayout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                   srcStages, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                   blitSource);
            }

            int32 width = GetWidth();
            int32 height = GetHeight();

            for (auto mip = firstToBlit; mip < firstToBlit + mipsToGen; mip++) {
                auto sourceLevel = mip - 1;
                int32 mipWidth = width > 1? width / 2: 1;
                int32 mipHeight = height > 1? height / 2: 1;

                VkImageSubresourceRange blitTarget{};
                blitTarget.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                blitTarget.baseMipLevel = mip;
                blitTarget.levelCount = 1;
                blitTarget.baseArrayLayer = 0;
                blitTarget.layerCount = arrayLayers;

                // 2. Transition layout of the mips targets, which we are going to generate
                // Initial layout does not matter, since we will override entire images

                VkAccessFlags srcFlags;
                VkPipelineStageFlags srcStages;
                GetSrcBarrierSettings(srcFlags, srcStages);

                utils.BarrierImage(buffer,
                                   mImage,
                                   srcFlags, VK_ACCESS_TRANSFER_WRITE_BIT,
                                   VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                   srcStages, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                   blitTarget);

                VkImageBlit blit{};
                blit.srcOffsets[0] = { 0, 0, 0 };
                blit.srcOffsets[1] = { width, height, 1 };
                blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                blit.srcSubresource.mipLevel = sourceLevel;
                blit.srcSubresource.baseArrayLayer = 0;
                blit.srcSubresource.layerCount = arrayLayers;
                blit.dstOffsets[0] = { 0, 0, 0 };
                blit.dstOffsets[1] = { mipWidth, mipHeight, 1 };
                blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                blit.dstSubresource.mipLevel = mip;
                blit.dstSubresource.baseArrayLayer = 0;
                blit.dstSubresource.layerCount = arrayLayers;

                // 3. Blit image (source mit to mip)
                vkCmdBlitImage(buffer,
                               mImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                               mImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               1, &blit, VK_FILTER_LINEAR);

                // 4. Transition layer to transfer read layout
                utils.BarrierImage(buffer,
                                   mImage,
                                   VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
                                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                   VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                   blitTarget);

                // Update width and height of the next source mip
                width = mipWidth;
                height = mipHeight;
            }

            {
                // 5. Transition back whole image to default layout
                auto range = GetTextureResourceRange();

                VkAccessFlags dstFlags;
                VkPipelineStageFlags dstStages;
                GetDstBarrierSetting(dstFlags, dstStages);

                utils.BarrierImage(buffer,
                                   mImage,
                                   VK_ACCESS_TRANSFER_READ_BIT, dstFlags,
                                   VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, mPrimaryLayout,
                                   VK_PIPELINE_STAGE_TRANSFER_BIT, dstStages,
                                   range);
            }

            BERSERK_VK_END_LABEL(buffer);
        }

        bool VulkanTexture::CanUpdate() const {
            auto usage = mDesc.textureUsage;
            return usage.Get(TextureUsage::Sampling) &&
                   !usage.Get(TextureUsage::DepthAttachment) &&
                   !usage.Get(TextureUsage::DepthStencilAttachment);
        }

        void VulkanTexture::GetSrcBarrierSettings(VkAccessFlags &access, VkPipelineStageFlags &stages) const {
            auto usage = GetTextureUsage();

            switch (mPrimaryLayout) {
                case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
                    if (usage.Get(TextureUsage::ColorAttachment) ||
                        usage.Get(TextureUsage::DepthAttachment) ||
                        usage.Get(TextureUsage::DepthStencilAttachment)) {
                        access = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                        stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                    } else {
                        access = VK_ACCESS_SHADER_READ_BIT;
                        stages = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                    }
                    break;
                case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
                    access = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                    stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                    break;
                case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
                    access = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                    stages = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
                    break;
                default:
                    access = 0;
                    stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                    break;
            }
        }

        void VulkanTexture::GetDstBarrierSetting(VkAccessFlags &access, VkPipelineStageFlags &stages) const {
            switch (mPrimaryLayout) {
                case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
                    access = VK_ACCESS_SHADER_READ_BIT;
                    stages = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
                    break;
                case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
                    access = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                    stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                    break;
                case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
                    access = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                    stages = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
                    break;
                default:
                    access = 0;
                    stages = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
                    break;
            }
        }

        VkImageSubresourceRange VulkanTexture::GetTextureResourceRange() const {
            VkImageSubresourceRange range{};
            range.layerCount = GetTextureType() == TextureType::TextureCube? Limits::MAX_TEXTURE_CUBE_FACES: GetArraySlices();
            range.baseArrayLayer = 0;
            range.baseMipLevel = 0;
            range.levelCount = GetMipsCount();
            range.aspectMask = VulkanDefs::GetAspectFlags(GetTextureFormat());

            return range;
        }

        void VulkanTexture::Validate2d() {
            assert(GetWidth() > 0);
            assert(GetHeight() > 0);
            assert(GetDepth() == 1);
            assert(GetArraySlices() == 1);
            assert(mDevice.GetSupportedFormats().Contains(GetTextureFormat()));
        }

        void VulkanTexture::Validate2dArray() {
            assert(GetWidth() > 0);
            assert(GetHeight() > 0);
            assert(GetDepth() == 1);
            assert(GetArraySlices() > 0);
            assert(mDevice.GetSupportedFormats().Contains(GetTextureFormat()));
        }

        void VulkanTexture::ValidateCube() {
            Validate2d();
        }

        void VulkanTexture::InitializeInternal() {
            auto range = GetTextureResourceRange();
            CreateImage();
            CreateView(range);
            TransitionToPrimaryLayout(range);
        }

        void VulkanTexture::CreateImage() {
            assert(GetMipsCount() <= PixelUtil::GetMaxMipsCount(GetWidth(), GetHeight(), GetDepth()));

            auto& queues = *mDevice.GetQueues();
            auto& queueFamilies = queues.GetUniqueFamilies();
            auto& memMan = *mDevice.GetMemoryManager();

            VkImageCreateInfo imageInfo{};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.pNext = nullptr;
            imageInfo.imageType = VulkanDefs::GetImageType(GetTextureType());
            imageInfo.extent.width = mDesc.width;
            imageInfo.extent.height = mDesc.height;
            imageInfo.extent.depth = mDesc.depth;
            imageInfo.mipLevels = mDesc.mipsCount;
            imageInfo.arrayLayers = GetTextureType() == TextureType::TextureCube? Limits::MAX_TEXTURE_CUBE_FACES: GetArraySlices();
            imageInfo.format = VulkanDefs::GetTextureFormat(GetTextureFormat());
            imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageInfo.usage = mUsageFlags;
            imageInfo.sharingMode = queues.GetResourcesSharingMode();
            imageInfo.queueFamilyIndexCount = queueFamilies.GetSize();
            imageInfo.pQueueFamilyIndices = queueFamilies.GetData();
            imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageInfo.flags = GetTextureType() == TextureType::TextureCube? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT: 0;

            auto allocation = memMan.AllocateImage(imageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            mImage = allocation.image;
            mImageAllocation = allocation.allocation;

            BERSERK_VK_NAME(mDevice.GetDevice(), mImage, VK_OBJECT_TYPE_IMAGE, "Image %name%");
        }

        void VulkanTexture::CreateView(const VkImageSubresourceRange &range) {
            auto format = GetTextureFormat();
            auto type = GetTextureType();

            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = mImage;
            viewInfo.viewType = VulkanDefs::GetImageViewType(type);
            viewInfo.format = VulkanDefs::GetTextureFormat(format);
            viewInfo.subresourceRange.aspectMask = VulkanDefs::GetAspectFlags(format);
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = GetMipsCount();
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = type == TextureType::TextureCube? Limits::MAX_TEXTURE_CUBE_FACES: GetArraySlices();
            viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            BERSERK_VK_CHECK(vkCreateImageView(mDevice.GetDevice(), &viewInfo, nullptr, &mView));
            BERSERK_VK_NAME(mDevice.GetDevice(), mView, VK_OBJECT_TYPE_IMAGE_VIEW, "ImageView %name%");
        }

        void VulkanTexture::TransitionToPrimaryLayout(const VkImageSubresourceRange &range) {
            auto& utils = *mDevice.GetUtils();
            auto buffer = mDevice.GetCmdBufferManager()->GetUploadCmdBuffer();

            VkAccessFlags dstFlags;
            VkPipelineStageFlags dstStages;
            GetDstBarrierSetting(dstFlags, dstStages);

            utils.BarrierImage(buffer,
                               mImage,
                               0, dstFlags,
                               VK_IMAGE_LAYOUT_UNDEFINED, mPrimaryLayout,
                               VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, dstStages,
                               range);
        }

        void VulkanTexture::UpdateTexture2DArrayInternal(VkCommandBuffer buffer, uint32 arrayIndex, uint32 mipLevel,
                                                         const Math::Rect2u &region, const PixelData &memory) {
            assert(VK_IMAGE_ASPECT_COLOR_BIT == VulkanDefs::GetAspectFlags(GetTextureFormat()));
            assert(mipLevel < GetMipsCount());
            assert(arrayIndex < GetArraySlices());
            assert(region.x() <= region.z());
            assert(region.y() <= region.w());
            assert(CanUpdate());

            auto mipSize = PixelUtil::GetMipSize(mipLevel, GetWidth(), GetHeight());
            assert(region.z() <= mipSize.x());
            assert(region.w() <= mipSize.y());

            auto& memMan = *mDevice.GetMemoryManager();
            auto vmaAlloc = memMan.GetVmaAllocator();

            const auto* pixels = memory.GetData();
            auto pixelDataSize = memory.GetDataSize();
            assert(VulkanDefs::CanCopyImage(GetTextureFormat(), memory.GetDataType(), memory.GetDataFormat()));

            // 1. Allocate staging buffer to transfer image
            auto staging = mDevice.GetStagePool()->AllocateBuffer(pixelDataSize);

            void* mappedRegion;
            BERSERK_VK_CHECK(vmaMapMemory(vmaAlloc, staging.allocation, &mappedRegion));

            // 2. Copy data into staging buffer
            Memory::Copy(mappedRegion, pixels, pixelDataSize);
            vmaUnmapMemory(vmaAlloc, staging.allocation);

            VkImageSubresourceRange subresource{};
            subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            subresource.baseMipLevel = mipLevel;
            subresource.levelCount = 1;
            subresource.baseArrayLayer = arrayIndex;
            subresource.layerCount = 1;

            int32 xoffset = region.x();
            int32 yoffset = region.y();
            uint32 width = region.z();
            uint32 height = region.w();

            VkBufferImageCopy copyRegion{};
            copyRegion.bufferOffset = 0;
            copyRegion.bufferRowLength = 0;
            copyRegion.bufferImageHeight = 0;
            copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copyRegion.imageSubresource.mipLevel = mipLevel;
            copyRegion.imageSubresource.baseArrayLayer = arrayIndex;
            copyRegion.imageSubresource.layerCount = 1;
            copyRegion.imageOffset = {xoffset, yoffset, 0};
            copyRegion.imageExtent = { width, height, 1 };

            // 3. Copy data into image (transition layout automatically)
            BERSERK_VK_BEGIN_LABEL(buffer, "UpdateTexture2D");
            UpdateSubResource(buffer, staging.buffer, copyRegion, subresource);
            BERSERK_VK_END_LABEL(buffer);
        }

    }
}