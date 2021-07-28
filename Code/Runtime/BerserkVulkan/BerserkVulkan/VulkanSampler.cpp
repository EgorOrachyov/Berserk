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

#include <BerserkVulkan/VulkanSampler.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanDebug.hpp>

namespace Berserk {
    namespace RHI {

        VulkanSampler::VulkanSampler(struct VulkanDevice &device, const Sampler::Desc &desc) : mDevice(device) {
            mState = desc;
        }

        VulkanSampler::~VulkanSampler() {
            if (mHandle) {
                BERSERK_VK_LOG_INFO(BERSERK_TEXT("Release Sampler: {0}"), mHandle);

                vkDestroySampler(mDevice.GetDevice(), mHandle, nullptr);
                mHandle = nullptr;
            }
        }

        void VulkanSampler::Initialize() {
            const auto& caps = mDevice.GetCaps();
            auto anisotropy = Math::Utils::Clamp(mState.maxAnisotropy, 1.0f, caps.maxAnisotropy);
            auto useAnisotropy = mState.useAnisotropy && caps.supportAnisotropy;

            VkFilter mag = VulkanDefs::GetMagFilter(mState.magFilter);
            VkFilter min;
            VkSamplerMipmapMode mipmapMode;
            VulkanDefs::GetMinFilter(mState.minFilter, min, mipmapMode);

            VkSamplerCreateInfo samplerInfo{};
            samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerInfo.pNext = nullptr;
            samplerInfo.magFilter = mag;
            samplerInfo.minFilter = min;
            samplerInfo.addressModeU = VulkanDefs::GetAddressMode(mState.u);
            samplerInfo.addressModeV = VulkanDefs::GetAddressMode(mState.v);
            samplerInfo.addressModeW = VulkanDefs::GetAddressMode(mState.w);
            samplerInfo.anisotropyEnable = useAnisotropy;
            samplerInfo.maxAnisotropy = anisotropy;
            samplerInfo.borderColor = VulkanDefs::GetBorderColor(mState.color);
            samplerInfo.unnormalizedCoordinates = VK_FALSE;
            samplerInfo.compareEnable = VK_FALSE;
            samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
            samplerInfo.mipmapMode = mipmapMode;
            samplerInfo.mipLodBias = 0.0f;
            samplerInfo.minLod = mState.minLod;
            samplerInfo.maxLod = mState.maxLod;

            BERSERK_VK_CHECK(vkCreateSampler(mDevice.GetDevice(), &samplerInfo, nullptr, &mHandle));

            BERSERK_VK_LOG_INFO(BERSERK_TEXT("Create Sampler: {0}"), mHandle);
        }


    }
}