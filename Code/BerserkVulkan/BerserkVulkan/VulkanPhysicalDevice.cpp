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

#include <BerserkVulkan/VulkanPhysicalDevice.hpp>
#include <BerserkVulkan/VulkanSurface.hpp>
#include <BerserkVulkan/VulkanQueues.hpp>
#include <BerserkCore/Error/Exception.hpp>

namespace Berserk {
    namespace RHI {

        VulkanPhysicalDevice::VulkanPhysicalDevice(VkInstance instance, const RefCounted<VulkanSurface>& surface, const Array <String> &extensions) {
            uint32 devicesCount;
            BERSERK_VK_CHECK(vkEnumeratePhysicalDevices(instance, &devicesCount, nullptr));

            assert(devicesCount > 0);

            if (devicesCount == 0) {
                BERSERK_VK_LOG_FATAL("No Vulkan-compatible physical device in the system");
                BERSERK_EXCEPT(FatalError, "No Vulkan-compatible physical device in the system");
            }

            Array<VkPhysicalDevice> availableDevices;
            availableDevices.Resize(devicesCount);
            BERSERK_VK_CHECK(vkEnumeratePhysicalDevices(instance, &devicesCount, availableDevices.GetData()));

            uint32 INVALID_DEVICE = devicesCount + 1;
            uint32 selectedDeviceScore = 0;
            uint32 selectedDevice = INVALID_DEVICE;

            for (uint32 currentDevice = 0; currentDevice < devicesCount; currentDevice++) {
                auto physicalDevice = availableDevices[currentDevice];
                uint32 currentScore = 0;

                VkPhysicalDeviceProperties deviceProperties;
                vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

                VkPhysicalDeviceFeatures deviceFeatures;
                vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

                /** Prefer discrete GPU if available */
                if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                    currentScore += 1000;
                }

                /** Integrated GPU not so bad  */
                if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
                    currentScore += 500;
                }

                /** Geometry shaders currently not used, but common its cool feature in future */
                if (deviceFeatures.geometryShader) {
                    currentScore += 1;
                }

                /** Check queues info */
                VulkanQueues queues(physicalDevice, surface->GetSurface());

                /** Queues set must be complete, otherwise we won't be able to draw and present something */
                if (!queues.IsComplete()) {
                    continue;
                }

                /** Check extensions */
                {
                    uint32 extensionsCount = 0;
                    Array<VkExtensionProperties> availableExtensions;
                    BERSERK_VK_CHECK(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, nullptr));

                    availableExtensions.Resize(extensionsCount);
                    BERSERK_VK_CHECK(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, availableExtensions.GetData()));

                    bool supported = true;

                    for (auto& required: extensions) {
                        bool found = false;

                        for (auto& available: availableExtensions) {
                            if (required == available.extensionName) {
                                found = true;
                                break;
                            }
                        }

                        if (!found) {
                            BERSERK_VK_LOG_WARNING(BERSERK_TEXT("Device {0} does not support required extension {1}"),
                                                   deviceProperties.deviceName, required);
                            supported = false;
                            break;
                        }
                    }

                    if (!supported) {
                        continue;
                    }
                }

                /** Check swap chain info */
                {
                    VulkanSwapChainSupportInfo supportInfo;
                    surface->GetSupportInfo(physicalDevice, queues.GetPresentQueueFamilyIndex(), supportInfo);

                    bool adequate = supportInfo.presentModes.IsNotEmpty() &&
                                    supportInfo.formats.IsNotEmpty() &&
                                    supportInfo.supportPresentation;

                    if (!adequate) {
                        continue;
                    }
                }

                /** Finally, device is OK, but we want the best one */
                if (currentScore > selectedDeviceScore) {
                    selectedDevice = currentDevice;
                    selectedDeviceScore = currentScore;
                }
            }

            if (selectedDevice == INVALID_DEVICE) {
                BERSERK_EXCEPT(FatalError, "Failed to select compatible physical device");
            }

            mPhysicalDevice = availableDevices[selectedDevice];
            vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &mMemoryProperties);

#ifdef BERSERK_DEBUG
            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(mPhysicalDevice, &deviceProperties);

            // 0x1002 (4098) - AMD
            // 0x1010 (4112) - ImgTec
            // 0x10DE (4318) - NVIDIA
            // 0x13B5 (5045) - ARM
            // 0x5143 (20803) - Qualcomm
            // 0x8086 (32902) - INTEL

            BERSERK_VK_LOG_INFO(BERSERK_TEXT("Vulkan Device: deviceName={0} deviceID={1} driverVersion={2} vendorID={3}"),
                                deviceProperties.deviceName, deviceProperties.deviceID, deviceProperties.driverVersion, deviceProperties.vendorID);
#endif
        }

        void VulkanPhysicalDevice::GetPhysicalDeviceFeatures(VkPhysicalDeviceFeatures &features) const {
            assert(mPhysicalDevice);
            vkGetPhysicalDeviceFeatures(mPhysicalDevice, &features);
        }

        void VulkanPhysicalDevice::GetSupportedFormats(Array<TextureFormat> &formats) const {
            TextureFormat candidates[] = {
                TextureFormat::R8,
                TextureFormat::R8_SNORM,
                TextureFormat::R16,
                TextureFormat::R16_SNORM,
                TextureFormat::RG8,
                TextureFormat::RG8_SNORM,
                TextureFormat::RG16,
                TextureFormat::RG16_SNORM,
                TextureFormat::RGB8,
                TextureFormat::RGB8_SNORM,
                TextureFormat::RGB16_SNORM,
                TextureFormat::RGBA8,
                TextureFormat::RGBA8_SNORM,
                TextureFormat::RGBA16,
                TextureFormat::SRGB8,
                TextureFormat::SRGB8_ALPHA8,
                TextureFormat::R16F,
                TextureFormat::RG16F,
                TextureFormat::RGB16F,
                TextureFormat::RGBA16F,
                TextureFormat::R32F,
                TextureFormat::RG32F,
                TextureFormat::RGB32F,
                TextureFormat::RGBA32F,
                TextureFormat::DEPTH32F,
                TextureFormat::DEPTH32F_STENCIL8,
                TextureFormat::DEPTH24_STENCIL8
            };

            VkFormatFeatureFlags defaultFeatures =
                    VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT |
                    VK_FORMAT_FEATURE_TRANSFER_SRC_BIT |
                    VK_FORMAT_FEATURE_TRANSFER_DST_BIT;

            VkFormatFeatureFlags colorAttachment =
                    defaultFeatures |
                    VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;

            VkFormatFeatureFlags depthStencilAttachment =
                    defaultFeatures |
                    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

            VkFormatFeatureFlags featureFlags[] = {
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    colorAttachment,
                    depthStencilAttachment,
                    depthStencilAttachment,
                    depthStencilAttachment
            };

            static_assert(sizeof(candidates) / sizeof(candidates[0]) == sizeof(featureFlags) / sizeof(featureFlags[0]));

            for (size_t i = 0; i < sizeof(candidates) / sizeof(candidates[0]); i++) {
                auto candidate = candidates[i];
                auto features = featureFlags[i];
                auto vkFormat = VulkanDefs::GetTextureFormat(candidate);

                VkFormatProperties props;
                vkGetPhysicalDeviceFormatProperties(mPhysicalDevice, vkFormat, &props);

                if ((props.optimalTilingFeatures & features) == features ||
                    (props.linearTilingFeatures & features) == features)
                    formats.Add(candidate);
            }
        }

    }
}