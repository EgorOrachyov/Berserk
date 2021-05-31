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

#ifndef BERSERK_VULKANDEVICE_HPP
#define BERSERK_VULKANDEVICE_HPP

#include <BerserkRHI/RHIDevice.hpp>
#include <BerserkVulkan/VulkanDefs.hpp>
#include <BerserkVulkan/VulkanUtils.hpp>

namespace Berserk {
    namespace RHI {

        /**
         * @brief Vulkan RHI Device
         *
         * This class is primary wrapper for vulkan logical device object. Its stores
         * global application vulkan instance, selected physical device, queue settings,
         * and created logical device.
         *
         * The engine won't support multi-GPU rendering with several devices, so currently
         * its is enough to store device + instance here and provide objets creation
         * RHI API from this class.
         */
        class VulkanDevice final: public Device {
        public:
            explicit VulkanDevice(VulkanDeviceInitInfo initStruct);
            VulkanDevice(const VulkanDevice&) = delete;
            VulkanDevice(VulkanDevice&&) = delete;
            ~VulkanDevice() override;

            RefCounted<VertexDeclaration> CreateVertexDeclaration(const VertexDeclaration::Desc &desc) override;
            RefCounted<VertexBuffer> CreateVertexBuffer(const VertexBuffer::Desc &desc) override;
            RefCounted<IndexBuffer> CreateIndexBuffer(const IndexBuffer::Desc &desc) override;
            RefCounted<UniformBuffer> CreateUniformBuffer(const UniformBuffer::Desc &desc) override;

            RefCounted<Sampler> CreateSampler(const Sampler::Desc &desc) override;
            RefCounted<Texture> CreateTexture(const Texture::Desc &desc) override;

            RefCounted<Framebuffer> CreateFramebuffer(const Framebuffer::Desc &desc) override;

            RefCounted<Program> CreateProgram(const Program::Desc &desc) override;
            RefCounted<CmdList> CreateCmdList() override;

            Type GetDriverType() const override;
            const DeviceCaps &GetCaps() const override;
            const Math::Mat4x4f &GetClipMatrix() const override;

        private:
            friend class VulkanSurface;
            friend class VulkanSurfaceManager;
            friend class VulkanProgram;
            friend class VulkanProgramCompiler;
            friend class VulkanPipelineCache;
            friend class VulkanFramebufferCache;
            friend class VulkanCmdBufferPool;
            friend class VulkanCmdBufferManager;
            friend class VulkanMemoryManager;
            friend class VulkanDescriptorSetManager;
            friend class VulkanStagePool;
            friend class VulkanBuffer;
            friend class VulkanContext;
            friend class VulkanUtils;
            friend class VulkanSampler;
            friend class VulkanTexture;

            VkInstance GetInstance() const { return mInstance; }
            VkDevice GetDevice() const { return mDevice; }

            const SharedPtr<class VulkanUtils> &GetUtils() { return mUtils; }
            const SharedPtr<class VulkanQueues> &GetQueues() const { return mQueues; }
            const SharedPtr<class VulkanPhysicalDevice> &GetPhysicalDevice() const { return mPhysicalDevice; }
            const SharedPtr<class VulkanSurfaceManager> &GetSurfaceManager() const { return mSurfaceManager; }
            const SharedPtr<class VulkanProgramCompiler> &GetCompiler() const { return mCompiler; }
            const SharedPtr<class VulkanCmdBufferPool> &GetCmdBufferPool() const { return mCmdBufferPool; }
            const SharedPtr<class VulkanCmdBufferManager> &GetCmdBufferManager() const { return mCmdBufferManager; }
            const SharedPtr<class VulkanMemoryManager> &GetMemoryManager() const { return mMemManager; }
            const SharedPtr<class VulkanStagePool> &GetStagePool() const { return mStagePool; }

            /** Advance device frame state (cause update of dependent systems) */
            void NextFrame();

            /** Wait on the host for the completion of outstanding queue operations for all queues */
            void WaitDeviceIdle();

        private:
            void CreateInstance();
            void CreateDevice();
            void ReleaseObjects();
            bool CheckExtensionsSupport(const Array<VkExtensionProperties> &available) const;
            bool CheckValidationLayersSupport(const Array<VkLayerProperties> &available) const;

            static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                                VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                                void* pUserData);
        private:
            // Config provided from user (plus requirements)
            String mApplicationName;
            String mEngineName;
            Array<String> mRequiredLayers;
            Array<String> mRequiredExtensions;
            Array<String> mRequiredDeviceExtensions;
            DeviceCaps mCaps{};
            Math::Mat4x4f mClipMatrix;
            bool mUseValidationLayers = false;

            // Vulkan objets managed by this class (ownership)
            VkInstance mInstance = nullptr;
            VkDevice mDevice = nullptr;
            VkDebugUtilsMessengerEXT mDebugMessenger = nullptr;

            // Globally accessible object by this device reference.This object is common
            // infrastructure, required for creating other objects and rendering management.
            SharedPtr<class VulkanUtils> mUtils;
            SharedPtr<class VulkanQueues> mQueues;
            SharedPtr<class VulkanPhysicalDevice> mPhysicalDevice;
            SharedPtr<class VulkanSurfaceManager> mSurfaceManager;
            SharedPtr<class VulkanProgramCompiler> mCompiler;
            SharedPtr<class VulkanCmdBufferPool> mCmdBufferPool;
            SharedPtr<class VulkanCmdBufferManager> mCmdBufferManager;
            SharedPtr<class VulkanMemoryManager> mMemManager;
            SharedPtr<class VulkanStagePool> mStagePool;
        };

    }
}

#endif //BERSERK_VULKANDEVICE_HPP