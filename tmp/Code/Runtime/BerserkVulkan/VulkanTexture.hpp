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

#ifndef BERSERK_VULKANTEXTURE_HPP
#define BERSERK_VULKANTEXTURE_HPP

#include <BerserkRHI/RHITexture.hpp>
#include <BerserkVulkan/VulkanDefs.hpp>
#include <BerserkVulkan/VulkanResource.hpp>
#include <BerserkVulkan/VulkanMemoryManager.hpp>

namespace Berserk {
    namespace RHI {

        class VulkanTexture : public Texture, public VulkanResource {
        public:
            explicit VulkanTexture(class VulkanDevice& device, const Desc& desc);
            ~VulkanTexture() override;

            void Initialize();
            void UpdateTexture2D(VkCommandBuffer buffer, uint32 mipLevel, const Math::Rect2u &region, const PixelData& memory);
            void UpdateTexture2DArray(VkCommandBuffer buffer, uint32 arrayIndex, uint32 mipLevel, const Math::Rect2u &region, const PixelData& memory);
            void UpdateTextureCube(VkCommandBuffer buffer, TextureCubemapFace face, uint32 mipLevel, const Math::Rect2u &region, const PixelData &memory);
            void UpdateSubResource(VkCommandBuffer buffer, VkBuffer staging, const VkBufferImageCopy& copy, const VkImageSubresourceRange& range);
            void GenerateMipmaps(VkCommandBuffer buffer);

            bool CanUpdate() const;
            void GetSrcBarrierSettings(VkAccessFlags& access, VkPipelineStageFlags& stages) const;
            void GetDstBarrierSetting(VkAccessFlags& access, VkPipelineStageFlags& stages) const;
            VkImageSubresourceRange GetTextureResourceRange() const;

            VkImage GetImage() const { return mImage; }
            VkImageView GetView() const { return mView; }
            VkImageLayout GetPrimaryLayout() const { return mPrimaryLayout; }

        private:
            void Validate2d();
            void Validate2dArray();
            void ValidateCube();
            void InitializeInternal();
            void CreateImage();
            void CreateView(const VkImageSubresourceRange &range);
            void TransitionToPrimaryLayout(const VkImageSubresourceRange& range);
            void UpdateTexture2DArrayInternal(VkCommandBuffer buffer, uint32 arrayIndex, uint32 mipLevel, const Math::Rect2u &region, const PixelData& memory);

        private:
            VkImage mImage = VK_NULL_HANDLE;
            VkImageView mView = VK_NULL_HANDLE;
            VkImageLayout mPrimaryLayout{};
            VkImageUsageFlags mUsageFlags = 0;
            VmaAllocation mImageAllocation = nullptr;

            class VulkanDevice& mDevice;
        };

    }
}

#endif //BERSERK_VULKANTEXTURE_HPP