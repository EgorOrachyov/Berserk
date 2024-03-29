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

#ifndef BERSERK_VULKANFRAMEBUFFERCACHE_HPP
#define BERSERK_VULKANFRAMEBUFFERCACHE_HPP

#include <BerserkRHI/RHIRenderPass.hpp>
#include <BerserkVulkan/VulkanFramebuffer.hpp>
#include <BerserkVulkan/VulkanDefs.hpp>
#include <BerserkVulkan/VulkanSurface.hpp>

namespace Berserk {
    namespace RHI {

        /**
         * @brief Framebuffer cache
         *
         * Cache for vulkan framebuffer objects and required for their creation
         * render passes. Since for different passes we potentially need to
         * create different fbos' we need to cache it here.
         *
         * Framebuffer here is effectively a binding of different attachments
         * with some settings. Actual texture targets are created by user
         * and provided in the VulkanFramebuffer object descriptor.
         */
        class VulkanFramebufferCache: public VulkanCache {
        public:

            /**
             * Render pass setup + (framebuffer|surface), provided by the user
             * (if window surface present, framebuffer == null (a.k.a default)).
             * If draw into surface, set frameIndex, to fetch fbo corresponding to specified image in the swap chain
             */
            struct RenderPassDescriptor {
                RenderPass renderPass;
                RcPtr<Framebuffer> framebuffer;
                RcPtr<VulkanSurface> surface;
                uint32 frameIndex = 0;
            };

            /**
             * Map render pass description, initial/final attachment layouts and formats.
             * Support only single subpass, so no more info is required.
             */
            struct RenderPassKey {
                ArrayFixed<RenderTargetOption, Limits::MAX_COLOR_ATTACHMENTS> colorAttachmentOptions;
                ArrayFixed<VkFormat, Limits::MAX_COLOR_ATTACHMENTS> colorAttachmentFormats;
                ArrayFixed<VkImageLayout, Limits::MAX_COLOR_ATTACHMENTS> colorAttachmentLayouts;
                RenderTargetOption depthOption;
                RenderTargetOption stencilOption;
                VkFormat depthStencilFormat;
                VkImageLayout depthStencilLayout;
                uint32 version;
                bool depth;
                bool stencil;
                CrcHash hash;
            }; // sizeof(RenderPassKey) ~ 104 bytes

            struct RenderPassValue {
                VkRenderPass renderPass = VK_NULL_HANDLE;
                uint32 frameUsed = 0;
            };

            /**
             * Vulkan framebuffer is built from a collection of actual texture attachments and
             * from render pass, which tells how to use this stuff in time of rendering.
             * So keep it here to map possible single VulkanFramebuffer to multiple VkFramebuffers
             */
            struct FramebufferKey {
                RcPtr<Framebuffer> framebuffer;
                RcPtr<VulkanSurface> surface;
                VkRenderPass renderPass;
                uint32 version;
                uint32 hash;
            };

            struct FramebufferValue {
                /** Frame buffer for each image in the swapchain */
                Array<VkFramebuffer> swapchain;
                /** Framebuffer for the simple user created fbo */
                VkFramebuffer framebuffer = VK_NULL_HANDLE;
                uint32 frameUsed = 0;
            };

            struct RenderPassObjects {
                /** For rendering */
                VkFramebuffer framebuffer;
                /** For pipeline creation */
                VkRenderPass renderPass;
            };

            explicit VulkanFramebufferCache(class VulkanDevice& device, uint32 releaseFrequency = RELEASE_FREQUENCY, uint32 timeToKeep = TIME_TO_KEEP);
            VulkanFramebufferCache(const VulkanFramebufferCache&) = delete;
            VulkanFramebufferCache(VulkanFramebufferCache&&) noexcept = delete;
            ~VulkanFramebufferCache();

            /** Attempts to find suitable set of render pass object, if failed creates new one */
            RenderPassObjects GetOrCreateRenderPass(const RenderPassDescriptor& descriptor);

            /** Releases cache accordingly to the settings (must be called every frame) */
            void GC();

        private:
            void CreateRenderPassKey(const RenderPassDescriptor& descriptor, RenderPassKey& key) const;
            void CreateRenderPassValue(const RenderPassKey& descriptor, const StringName& name, RenderPassValue& value) const;
            void ReleaseRenderPass(RenderPassValue& value) const;

            void CreateFramebufferKey(const RenderPassDescriptor& descriptor, VkRenderPass renderPass, FramebufferKey& key) const;
            void CreateFramebufferValue(const FramebufferKey& descriptor, const StringName& name, FramebufferValue& value) const;
            void ReleaseFramebuffer(FramebufferValue& value) const;

            HashTable<RenderPassKey, RenderPassValue> mRenderPasses;
            HashTable<FramebufferKey, FramebufferValue> mFramebuffers;
        };

    }
}

#endif //BERSERK_VULKANFRAMEBUFFERCACHE_HPP