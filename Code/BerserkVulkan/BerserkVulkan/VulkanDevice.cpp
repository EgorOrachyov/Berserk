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

#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanCmdList.hpp>
#include <BerserkVulkan/VulkanSampler.hpp>
#include <BerserkVulkan/VulkanTexture.hpp>
#include <BerserkVulkan/VulkanUniformBuffer.hpp>
#include <BerserkVulkan/VulkanIndexBuffer.hpp>
#include <BerserkVulkan/VulkanVertexBuffer.hpp>
#include <BerserkVulkan/VulkanVertexDeclaration.hpp>
#include <BerserkVulkan/VulkanDebug.hpp>
#include <BerserkVulkan/VulkanQueues.hpp>
#include <BerserkVulkan/VulkanSurfaceManager.hpp>
#include <BerserkVulkan/VulkanPhysicalDevice.hpp>
#include <BerserkVulkan/VulkanProgramCompiler.hpp>
#include <BerserkVulkan/VulkanPipelineCache.hpp>
#include <BerserkVulkan/VulkanCmdBufferPool.hpp>
#include <BerserkVulkan/VulkanCmdBufferManager.hpp>
#include <BerserkVulkan/VulkanMemoryManager.hpp>
#include <BerserkVulkan/VulkanStagePool.hpp>
#include <BerserkVulkan/VulkanProxy.hpp>

namespace Berserk {
    namespace RHI {

        static void Pack(const Array<String>& source, Array<const char*> &destination) {
            destination.Clear();
            destination.EnsureToAdd(source.GetSize());

            for (auto& ext: source) {
                destination.Add(ext.GetStr_C());
            }
        }

        VulkanDevice::VulkanDevice(VulkanDeviceInitInfo initStruct) {
            mApplicationName = std::move(initStruct.applicationName);
            mEngineName = std::move(initStruct.engineName);
            mRequiredExtensions = std::move(initStruct.requiredExtensions);

            if (mUseValidationLayers) {
                mRequiredLayers.Add("VK_LAYER_KHRONOS_validation");
                mRequiredExtensions.Add(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }

            mRequiredDeviceExtensions.Add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

            try {
                // Instance, validation layers and debug callbacks setup
                CreateInstance();

                // Create surface manager and surface for primary window
                mSurfaceManager = SharedPtr<VulkanSurfaceManager>::Make(*this, initStruct);
                auto surface = mSurfaceManager->GetPrimarySurface();

                // Select physical device
                mPhysicalDevice = SharedPtr<VulkanPhysicalDevice>::Make(mInstance, surface, mRequiredDeviceExtensions);

                // When device is selected, get queue families info
                mQueues = SharedPtr<VulkanQueues>::Make(mPhysicalDevice->Get(), surface->GetSurface());

                // Create logical device with all required features
                CreateDevice();

                // Setup utils
                mUtils = SharedPtr<VulkanUtils>::Make(*this);

                // Setup queues after device is created
                mQueues->SetupQueuesFromDevice(mDevice);

                // Create glsl to spir-v async program compiler
                mCompiler = SharedPtr<VulkanProgramCompiler>::Make(*this);

                // Command buffers pool for graphics, transfer and presentation buffer allocation
                mCmdBufferPool = SharedPtr<VulkanCmdBufferPool>::Make(*this);
                mCmdBufferManager = SharedPtr<VulkanCmdBufferManager>::Make(*this);

                // Memory manager for general purpose staging and device allocations
                mMemManager = SharedPtr<VulkanMemoryManager>::Make(*this);

                // Staging pool for data CPU -> GPU transfer
                mStagePool = SharedPtr<VulkanStagePool>::Make(*this);
            }
            catch (Exception& exception) {
                ReleaseObjects();
                // We need to release created objects
                throw;
            }

            // Fill supported languages
            mSupportedShaderLanguages.Add(ShaderLanguage::GLSL450VK);

            // Fill supported texture formats
            mPhysicalDevice->GetSupportedFormats(mSupportedTextureFormats);

            // Fill device capabilities/limits
            // ................................

            // Finally, remove tmp primary partially created surface
            // (will be finally initialize on first scene rendering)
            mSurfaceManager->RemoveSurface(initStruct.primaryWindow);
        }

        VulkanDevice::~VulkanDevice() {
            ReleaseObjects();
        }

        RefCounted<VertexDeclaration> VulkanDevice::CreateVertexDeclaration(const VertexDeclaration::Desc &desc) {
            auto declaration = Memory::Make<VulkanVertexDeclaration>(desc);
            return RefCounted<VertexDeclaration>(declaration);
        }

        RefCounted<VertexBuffer> VulkanDevice::CreateVertexBuffer(const VertexBuffer::Desc &desc) {
            using ProxyVulkanBuffer = ResourceProxy<VulkanVertexBuffer>;
            auto vertexBuffer = Memory::Make<ProxyVulkanBuffer>(*this, desc);
            vertexBuffer->DeferredInit();
            return RefCounted<VertexBuffer>(vertexBuffer);
        }

        RefCounted<IndexBuffer> VulkanDevice::CreateIndexBuffer(const IndexBuffer::Desc &desc) {
            using ProxyIndexBuffer = ResourceProxy<VulkanIndexBuffer>;
            auto indexBuffer = Memory::Make<ProxyIndexBuffer>(*this, desc);
            indexBuffer->DeferredInit();
            return RefCounted<IndexBuffer>(indexBuffer);
        }

        RefCounted<UniformBuffer> VulkanDevice::CreateUniformBuffer(const UniformBuffer::Desc &desc) {
            using ProxyUniformBuffer = ResourceProxy<VulkanUniformBuffer>;
            auto uniformBuffer = Memory::Make<ProxyUniformBuffer>(*this, desc);
            uniformBuffer->DeferredInit();
            return RefCounted<UniformBuffer>(uniformBuffer);
        }

        RefCounted<Sampler> VulkanDevice::CreateSampler(const Sampler::Desc &desc) {
            using ProxySampler = ResourceProxy<VulkanSampler>;
            auto sampler = Memory::Make<ProxySampler>(*this, desc);
            sampler->DeferredInit();
            return RefCounted<Sampler>(sampler);
        }

        RefCounted<Texture> VulkanDevice::CreateTexture(const Texture::Desc &desc) {
            using ProxyTexture = ResourceProxy<VulkanTexture>;
            auto texture = Memory::Make<ProxyTexture>(*this, desc);
            texture->DeferredInit();
            return RefCounted<Texture>(texture);
        }

        RefCounted<Framebuffer> VulkanDevice::CreateFramebuffer(const Framebuffer::Desc &desc) {
            return RefCounted<Framebuffer>();
        }

        RefCounted<Program> VulkanDevice::CreateProgram(const Program::Desc &desc) {
            return mCompiler->CreateProgram(desc);
        }

        RefCounted<CmdList> VulkanDevice::CreateCmdList() {
            auto commandQueue = VulkanDriver::GetCommandQueue();
            auto& context = VulkanDriver::GetContext();
            auto cmdList = Memory::Make<VulkanCmdList>(std::move(commandQueue), context);
            return RefCounted<CmdList>(cmdList);
        }

        Type VulkanDevice::GetDriverType() const {
            return Type::Vulkan;
        }

        const DeviceCaps &VulkanDevice::GetCaps() const {
            return mCaps;
        }

        void VulkanDevice::CreateInstance() {
            Array<VkExtensionProperties> supportedExtensions;
            Array<VkLayerProperties> supportedLayers;

            uint32 extensionsCount = 0;
            BERSERK_VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr));

            supportedExtensions.Resize(extensionsCount);
            BERSERK_VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, supportedExtensions.GetData()));

            assert(CheckExtensionsSupport(supportedExtensions));

            if (mUseValidationLayers) {
                uint32 layersCount = 0;
                BERSERK_VK_CHECK(vkEnumerateInstanceLayerProperties(&layersCount, nullptr));
                supportedLayers.Resize(layersCount);

                BERSERK_VK_CHECK(vkEnumerateInstanceLayerProperties(&layersCount, supportedLayers.GetData()));
                assert(CheckValidationLayersSupport(supportedLayers));
            }

            Array<const char*> extensions;
            Pack(mRequiredExtensions, extensions);

            Array<const char*> layers;
            Pack(mRequiredLayers, layers);

            VkDebugUtilsMessengerCreateInfoEXT createInfoExt{};
            createInfoExt.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfoExt.pNext = nullptr;
            createInfoExt.messageSeverity =
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfoExt.messageType =
                    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfoExt.pfnUserCallback = DebugCallback;
            createInfoExt.pUserData = nullptr;

            VkApplicationInfo appInfo{};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = mApplicationName.GetStr_C();
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.pEngineName = mEngineName.GetStr_C();
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.apiVersion = VULKAN_VERSION;

            VkInstanceCreateInfo instanceCreateInfo{};
            instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            instanceCreateInfo.pApplicationInfo = &appInfo;
            instanceCreateInfo.enabledExtensionCount = extensions.GetSize();
            instanceCreateInfo.ppEnabledExtensionNames = extensions.GetData();
            instanceCreateInfo.enabledLayerCount = layers.GetSize();
            instanceCreateInfo.ppEnabledLayerNames = layers.GetData();
            instanceCreateInfo.pNext = nullptr;
            instanceCreateInfo.pNext = mUseValidationLayers? &createInfoExt: nullptr;

            BERSERK_VK_CHECK(vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance));


            if (mUseValidationLayers) {
                // Load instance related functions for debugging
                VulkanDebug::LoadInstanceFunctions(mInstance);

                // Setup deb messenger for the rest of API calls
                BERSERK_VK_CHECK(VulkanDebug::vkCreateDebugUtilsMessengerEXT(mInstance, &createInfoExt, nullptr, &mDebugMessenger));
            }
        }

        void VulkanDevice::CreateDevice() {
            // Enable all features
            VkPhysicalDeviceFeatures features{};
            mPhysicalDevice->GetPhysicalDeviceFeatures(features);

            Array<float> graphicsQueuesPriority;
            Array<float> transferQueuesPriority;
            Array<float> presentQueuesPriority;
            Array<VkDeviceQueueCreateInfo> queueCreateInfos;
            mQueues->GetQueuesCreateInfos(queueCreateInfos, graphicsQueuesPriority, transferQueuesPriority, presentQueuesPriority);

            Array<const char*> deviceExtensions;
            Pack(mRequiredDeviceExtensions, deviceExtensions);

            VkDeviceCreateInfo deviceCreateInfo{};
            deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.GetData();
            deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.GetSize();
            deviceCreateInfo.pEnabledFeatures = &features;
            deviceCreateInfo.enabledExtensionCount = deviceExtensions.GetSize();
            deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.GetData();

            Array<const char*> layers;

            if (mUseValidationLayers) {
                Pack(mRequiredLayers, layers);

                deviceCreateInfo.enabledLayerCount = layers.GetSize();
                deviceCreateInfo.ppEnabledLayerNames = layers.GetData();
            }

            BERSERK_VK_CHECK(vkCreateDevice(mPhysicalDevice->Get(), &deviceCreateInfo, nullptr, &mDevice));
        }

        void VulkanDevice::ReleaseObjects() {
            if (mInstance) {
                mSurfaceManager = nullptr;
                mCmdBufferManager = nullptr;
                mStagePool = nullptr;
                mMemManager = nullptr;
                mCmdBufferPool = nullptr;
                mCompiler = nullptr;
                mUtils = nullptr;

                if (mDevice)
                    vkDestroyDevice(mDevice, nullptr);

                mQueues = nullptr;
                mPhysicalDevice = nullptr;

                if (mDebugMessenger)
                    VulkanDebug::vkDestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);

                vkDestroyInstance(mInstance, nullptr);
            }
        }

        bool VulkanDevice::CheckExtensionsSupport(const Array<VkExtensionProperties> &available) const {
            bool allIncluded = true;

            for (auto& ext: mRequiredExtensions) {
                bool found = false;

                for (auto& supported: available) {
                    if (ext == supported.extensionName) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Failed to find extension {0}"), ext);
                    allIncluded = false;
                }
            }

            return allIncluded;
        }

        bool VulkanDevice::CheckValidationLayersSupport(const Array<VkLayerProperties> &available) const {
            bool allIncluded = true;

            for (auto& layer: mRequiredLayers) {
                bool found = false;

                for (auto& supported: available) {
                    if (layer == supported.layerName) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Failed to find layer {0}"), layer);
                    allIncluded = false;
                }
            }

            return allIncluded;
        }

        VkBool32 VulkanDevice::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                             VkDebugUtilsMessageTypeFlagsEXT messageType,
                                             const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                             void *pUserData) {
            switch (messageSeverity) {
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                    BERSERK_VK_LOG_INFO(BERSERK_TEXT("Validation Layer: {0}: {1}: {2}"),
                                        pCallbackData->messageIdNumber,
                                        pCallbackData->pMessageIdName,
                                        pCallbackData->pMessage);
                    break;
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                    BERSERK_VK_LOG_WARNING(BERSERK_TEXT("Validation Layer: {0}: {1}: {2}"),
                                           pCallbackData->messageIdNumber,
                                           pCallbackData->pMessageIdName,
                                           pCallbackData->pMessage);
                    break;
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                    BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Validation Layer: {0}: {1}: {2}"),
                                         pCallbackData->messageIdNumber,
                                         pCallbackData->pMessageIdName,
                                         pCallbackData->pMessage);
                    break;
                default:
                    break;
            }

            return VK_FALSE;
        }

        void VulkanDevice::NextFrame() {
            mCmdBufferPool->NextFrame();
            mMemManager->NextFrame();
            mStagePool->NextFrame();
        }


        void VulkanDevice::WaitDeviceIdle() {
            BERSERK_VK_CHECK(vkDeviceWaitIdle(mDevice));
        }

    }
}