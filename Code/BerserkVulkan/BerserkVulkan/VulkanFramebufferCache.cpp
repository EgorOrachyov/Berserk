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

#include <BerserkVulkan/VulkanFramebufferCache.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanDebug.hpp>

namespace Berserk {

    template<>
    struct Equals<RHI::VulkanFramebufferCache::RenderPassKey> {
    public:
        bool operator()(const RHI::VulkanFramebufferCache::RenderPassKey& a, const RHI::VulkanFramebufferCache::RenderPassKey& b) const {
            if (a.hash != b.hash)
                return false;

            auto aAttachmentsCount = a.colorAttachmentOptions.GetSize();
            auto bAttachmentsCount = b.colorAttachmentOptions.GetSize();

            if (aAttachmentsCount != bAttachmentsCount)
                return false;

            auto count = aAttachmentsCount;

            for (size_t i = 0; i < count; i++)
                if (a.colorAttachmentOptions[i] != b.colorAttachmentOptions[i])
                    return false;

            for (size_t i = 0; i < count; i++)
                if (a.colorAttachmentFormats[i] != b.colorAttachmentFormats[i])
                    return false;

            for (size_t i = 0; i < count; i++)
                if (a.colorAttachmentLayouts[i] != b.colorAttachmentLayouts[i])
                    return false;

            return a.version == b.version &&
                   a.depthOption == b.depthOption &&
                   a.stencilOption == b.stencilOption &&
                   a.depthStencilFormat == b.depthStencilFormat &&
                   a.depthStencilLayout == b.depthStencilLayout &&
                   a.depth == b.depth &&
                   a.stencil == b.stencil;
        }
    };

    template<>
    class Hash<RHI::VulkanFramebufferCache::RenderPassKey> {
    public:
        uint32 operator()(const RHI::VulkanFramebufferCache::RenderPassKey& key) const {
            return key.hash;
        }
    };

    template<>
    class Equals<RHI::VulkanFramebufferCache::FramebufferKey> {
    public:
        bool operator()(const RHI::VulkanFramebufferCache::FramebufferKey& a, const RHI::VulkanFramebufferCache::FramebufferKey& b) const {
            return a.hash == b.hash &&
                   a.version == b.version &&
                   a.framebuffer == b.framebuffer &&
                   a.surface == b.surface &&
                   a.renderPass == b.renderPass;
        }
    };

    template<>
    class Hash<RHI::VulkanFramebufferCache::FramebufferKey> {
    public:
        uint32 operator()(const RHI::VulkanFramebufferCache::FramebufferKey& key) const {
            return key.hash;
        }
    };

    namespace RHI {

        static CrcHash HashRenderPassKey(const VulkanFramebufferCache::RenderPassKey& key) {
            Crc32Builder builder;
            builder.Hash(&key, sizeof(VulkanFramebufferCache::RenderPassKey));
            return builder.GetHash();
        }

        static CrcHash HashFramebufferKey(const VulkanFramebufferCache::FramebufferKey& key) {
            Crc32Builder builder;
            builder.Hash(&key, sizeof(VulkanFramebufferCache::FramebufferKey));
            return builder.GetHash();
        }

        VulkanFramebufferCache::VulkanFramebufferCache(struct VulkanDevice &device, uint32 releaseFrequency, uint32 timeToKeep)
                : VulkanCache(device, releaseFrequency, timeToKeep) {
        }

        VulkanFramebufferCache::~VulkanFramebufferCache() {
            for (auto& entry: mFramebuffers) {
                auto& value = entry.GetSecond();
                ReleaseFramebuffer(value);
            }
            for (auto& entry: mRenderPasses) {
                auto& value = entry.GetSecond();
                ReleaseRenderPass(value);
            }
        }

        VulkanFramebufferCache::RenderPassObjects VulkanFramebufferCache::GetOrCreateRenderPass(const VulkanFramebufferCache::RenderPassDescriptor &descriptor) {
            RenderPassObjects objects{};

            // Look-up for render pass, if failed to find, create new one
            auto targetSurface = descriptor.surface.IsNotNull();
            auto index = descriptor.frameIndex;

            RenderPassKey renderPassKey;
            CreateRenderPassKey(descriptor, renderPassKey);

            auto rpValuePtr = mRenderPasses.GetPtr(renderPassKey);

            if (rpValuePtr != nullptr) {
                objects.renderPass = rpValuePtr->renderPass;
                rpValuePtr->frameUsed = mCurrentFrame;
            } else {
                RenderPassValue renderPassValue;
                StringName debugName = targetSurface? descriptor.surface->GetName(): descriptor.framebuffer->GetName();
                CreateRenderPassValue(renderPassKey, debugName, renderPassValue);

                mRenderPasses.Add(renderPassKey, renderPassValue);
                objects.renderPass = renderPassValue.renderPass;
            }

            // Now we have render pass, need to find or create suitable framebuffer

            FramebufferKey framebufferKey;
            CreateFramebufferKey(descriptor, objects.renderPass, framebufferKey);

            auto fboValuePtr = mFramebuffers.GetPtr(framebufferKey);

            if (fboValuePtr != nullptr) {
                fboValuePtr->frameUsed = mCurrentFrame;
                objects.framebuffer = targetSurface? fboValuePtr->swapchain[index]: fboValuePtr->framebuffer;
            } else {
                FramebufferValue framebufferValue;
                StringName debugName = targetSurface? descriptor.surface->GetName(): descriptor.framebuffer->GetName();
                CreateFramebufferValue(framebufferKey, debugName, framebufferValue);

                mFramebuffers.Add(framebufferKey, framebufferValue);
                objects.framebuffer = targetSurface? framebufferValue.swapchain[index]: framebufferValue.framebuffer;
            }

            return objects;
        }

        void VulkanFramebufferCache::GC() {
            if (mCurrentFrame >= mLastFrameRelease + mReleaseFrequency) {
                mLastFrameRelease = mCurrentFrame;

                auto currentRenderPass = mRenderPasses.begin();
                while (currentRenderPass != mRenderPasses.end()) {
                    auto& e = *currentRenderPass;

                    if (e.GetSecond().frameUsed + mTimeToKeep <= mCurrentFrame) {
                        ReleaseRenderPass(e.GetSecond());
                        currentRenderPass = mRenderPasses.Remove(currentRenderPass);
                    }
                    else
                        ++currentRenderPass;
                }

                auto currentFbo = mFramebuffers.begin();
                while (currentFbo != mFramebuffers.end()) {
                    auto& e = *currentFbo;

                    if (e.GetSecond().frameUsed + mTimeToKeep <= mCurrentFrame) {
                        ReleaseFramebuffer(e.GetSecond());
                        currentFbo = mFramebuffers.Remove(currentFbo);
                    }
                    else
                        ++currentFbo;
                }
            }

            mCurrentFrame += 1;
        }

        void VulkanFramebufferCache::CreateRenderPassKey(const VulkanFramebufferCache::RenderPassDescriptor &descriptor, VulkanFramebufferCache::RenderPassKey &key) const {
            assert((descriptor.framebuffer.IsNotNull() && descriptor.surface.IsNull()) ||
                   (descriptor.framebuffer.IsNull() && descriptor.surface.IsNotNull()));

            Memory::Set(&key, 0x0, sizeof(RenderPassKey));

            auto& renderPass = descriptor.renderPass;
            auto& fbo = descriptor.framebuffer;
            auto& color = renderPass.colorAttachments;
            auto& depthStencil = renderPass.depthStencilAttachment;

            if (fbo.IsNotNull()) {
                // we render into user created framebuffer

                auto vkFbo = (VulkanFramebuffer*) fbo.Get();

                vkFbo->GetLayouts(key.colorAttachmentLayouts, key.depthStencilLayout);
                vkFbo->GetFormats(key.colorAttachmentFormats, key.depthStencilFormat);

                for (auto& attachment: color)
                    key.colorAttachmentOptions.Add(attachment.option);

                key.depthOption = depthStencil.depthOption;
                key.stencilOption = depthStencil.stencilOption;

                key.depth = vkFbo->HasDepthBuffer();
                key.stencil = vkFbo->HasStencilBuffer();
                key.version = 0;
            } else {
                // we render into system surface
                assert(color.GetSize() == 1);

                auto attachment0 = color[0];
                auto& surface = descriptor.surface;

                key.colorAttachmentOptions.Add(attachment0.option);
                key.colorAttachmentFormats.Add(surface->GetFormat().format);
                key.colorAttachmentLayouts.Add(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

                key.depthOption = depthStencil.depthOption;
                key.stencilOption = depthStencil.stencilOption;
                key.depthStencilFormat = surface->GetDepthStencilFormat();
                key.depthStencilLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                key.depth = true;
                key.stencil = true;
                key.version = surface->GetVersion();
            }

            key.hash = HashRenderPassKey(key);
        }

        void VulkanFramebufferCache::CreateRenderPassValue(const VulkanFramebufferCache::RenderPassKey &descriptor, const StringName& name, VulkanFramebufferCache::RenderPassValue &value) const {
            ArrayFixed<VkAttachmentDescription, Limits::MAX_COLOR_ATTACHMENTS + 1> attachments;

            auto colorAttachmentsCount = descriptor.colorAttachmentOptions.GetSize();

            for (size_t i = 0; i < colorAttachmentsCount; i++) {
                auto option = descriptor.colorAttachmentOptions[i];
                auto format = descriptor.colorAttachmentFormats[i];
                auto layout = descriptor.colorAttachmentLayouts[i];

                // Define source and dest layout based on the base layout and options

                VkAttachmentLoadOp loadOp;
                VkAttachmentStoreOp storeOp;
                VulkanDefs::GetLoadStoreOps(option, loadOp, storeOp);

                VkAttachmentDescription& attachment = attachments.Emplace();
                attachment.format = format;
                attachment.samples = VK_SAMPLE_COUNT_1_BIT;
                attachment.loadOp = loadOp;
                attachment.storeOp = storeOp;
                attachment.initialLayout = VulkanDefs::DiscardsOnStart(option)? VK_IMAGE_LAYOUT_UNDEFINED: layout;
                attachment.finalLayout = layout;
            }

            auto depth = descriptor.depth;
            auto stencil = descriptor.stencil;
            auto depthStencil = depth || stencil;

            if (depthStencil) {
                auto depthOption = descriptor.depthOption;
                auto stencilOption = descriptor.stencilOption;
                auto format = descriptor.depthStencilFormat;
                auto layout = descriptor.depthStencilLayout;

                // Define layout based on depth and stencil ops at the same time

                VkAttachmentLoadOp loadOp;
                VkAttachmentStoreOp storeOp;
                VulkanDefs::GetLoadStoreOps(depthOption, loadOp, storeOp);

                VkAttachmentLoadOp stencilLoadOp;
                VkAttachmentStoreOp stencilStoreOp;
                VulkanDefs::GetLoadStoreOps(stencilOption, stencilLoadOp, stencilStoreOp);

                auto discardOnStart =
                        VulkanDefs::DiscardsOnStart(depthOption) &&
                        VulkanDefs::DiscardsOnStart(stencilOption);

                VkAttachmentDescription& depthStencilAttachment = attachments.Emplace();
                depthStencilAttachment.format = format;
                depthStencilAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
                depthStencilAttachment.loadOp = loadOp;
                depthStencilAttachment.storeOp = storeOp;
                depthStencilAttachment.stencilLoadOp = stencilLoadOp;
                depthStencilAttachment.stencilStoreOp = stencilStoreOp;
                depthStencilAttachment.initialLayout = discardOnStart? VK_IMAGE_LAYOUT_UNDEFINED: layout;
                depthStencilAttachment.finalLayout = layout;
            }

            ArrayFixed<VkAttachmentReference, Limits::MAX_COLOR_ATTACHMENTS> references;

            // Support only single subpass for now, so used only as color/depth/stencil attachments

            for (size_t i = 0; i < colorAttachmentsCount; i++) {
                VkAttachmentReference& reference = references.Emplace();
                reference.attachment = i;
                reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            }

            VkAttachmentReference depthStencilReference{};

            if (depthStencil) {
                // If presented, it is placed as last element, so take as [size - 1]
                depthStencilReference.attachment = attachments.GetSize() - 1;
                depthStencilReference.layout = VulkanDefs::GetDepthStencilSubpassLayout(depth, stencil);
            }

            VkSubpassDescription subpass{};
            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass.colorAttachmentCount = references.GetSize();
            subpass.pColorAttachments = references.GetData();
            subpass.pDepthStencilAttachment = &depthStencilReference;

            // todo: add possibility to describe subpasses (and dependencies)
            ArrayFixed<VkSubpassDependency, 2> dependencies;
            dependencies.Resize(2);

            dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
            dependencies[0].dstSubpass = 0;
            dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

            dependencies[1].srcSubpass = 0;
            dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
            dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dependencies[1].dstStageMask = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
            dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

            VkRenderPassCreateInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassInfo.attachmentCount = attachments.GetSize();
            renderPassInfo.pAttachments = attachments.GetData();
            renderPassInfo.subpassCount = 1;
            renderPassInfo.pSubpasses = &subpass;
            renderPassInfo.dependencyCount = dependencies.GetSize();
            renderPassInfo.pDependencies = dependencies.GetData();

            VkRenderPass renderPass;

            BERSERK_VK_CHECK(vkCreateRenderPass(mDevice.GetDevice(), &renderPassInfo, nullptr, &renderPass));
            BERSERK_VK_NAME(mDevice.GetDevice(), renderPass, VK_OBJECT_TYPE_RENDER_PASS, "RenderPass for " + name);

            value.renderPass = renderPass;
            value.frameUsed = mCurrentFrame;

            BERSERK_VK_LOG_INFO(BERSERK_TEXT("Cache new RenderPass: {0} {1}"), renderPass, name);
        }

        void VulkanFramebufferCache::ReleaseRenderPass(VulkanFramebufferCache::RenderPassValue &value) const {
            BERSERK_VK_LOG_INFO(BERSERK_TEXT("Release RenderPass: {0}"), value.renderPass);

            vkDestroyRenderPass(mDevice.GetDevice(), value.renderPass, nullptr);

            value.renderPass = nullptr;
            value.frameUsed = 0;
        }

        void VulkanFramebufferCache::CreateFramebufferKey(const VulkanFramebufferCache::RenderPassDescriptor &descriptor, VkRenderPass renderPass, VulkanFramebufferCache::FramebufferKey &key) const {
            Memory::Set(&key, 0x0, sizeof(FramebufferKey));

            key.framebuffer = descriptor.framebuffer;
            key.surface = descriptor.surface;
            key.renderPass = renderPass;
            key.hash = HashFramebufferKey(key);
            key.version = descriptor.surface? descriptor.surface->GetVersion(): 0;
        }

        void VulkanFramebufferCache::CreateFramebufferValue(const VulkanFramebufferCache::FramebufferKey &descriptor, const StringName &name, VulkanFramebufferCache::FramebufferValue &value) const {
            auto& fbo = descriptor.framebuffer;

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = descriptor.renderPass;

            if (fbo.IsNotNull()) {
                auto vkFbo = (VulkanFramebuffer*) fbo.Get();

                ArrayFixed<VkImageView, Limits::MAX_COLOR_ATTACHMENTS> colorAttachments;
                VkImageView depthStencilAttachment;

                vkFbo->GetViews(colorAttachments, depthStencilAttachment);

                ArrayFixed<VkImageView, Limits::MAX_COLOR_ATTACHMENTS + 1> attachments;

                for (auto view: colorAttachments)
                    attachments.Add(view);

                if (vkFbo->HasDepthBuffer() || vkFbo->HasStencilBuffer())
                    attachments.Add(depthStencilAttachment);

                framebufferInfo.attachmentCount = attachments.GetSize();
                framebufferInfo.pAttachments = attachments.GetData();
                framebufferInfo.width = vkFbo->GetWidth();
                framebufferInfo.height = vkFbo->GetHeight();
                framebufferInfo.layers = 1;

                VkFramebuffer framebuffer;

                BERSERK_VK_CHECK(vkCreateFramebuffer(mDevice.GetDevice(), &framebufferInfo, nullptr, &framebuffer));
                BERSERK_VK_NAME(mDevice.GetDevice(), framebuffer, VK_OBJECT_TYPE_FRAMEBUFFER, "Framebuffer for " + name);

                value.framebuffer = framebuffer;
            } else {
                auto& surface = descriptor.surface;

                auto& colors = surface->GetSwapColorImageViews();
                auto& depthStencils = surface->GetSwapDepthStencilImageViews();

                framebufferInfo.width = surface->GetWidth();
                framebufferInfo.height = surface->GetHeight();
                framebufferInfo.layers = 1;

                ArrayFixed<VkImageView, 2> attachments;

                for (size_t i = 0; i < surface->GetFramesCount(); i++) {
                    attachments.Clear();
                    attachments.Add(colors[i]);
                    attachments.Add(depthStencils[i]);

                    framebufferInfo.attachmentCount = attachments.GetSize();
                    framebufferInfo.pAttachments = attachments.GetData();

                    VkFramebuffer framebuffer;

                    BERSERK_VK_CHECK(vkCreateFramebuffer(mDevice.GetDevice(), &framebufferInfo, nullptr, &framebuffer));
                    BERSERK_VK_NAME(mDevice.GetDevice(), framebuffer, VK_OBJECT_TYPE_FRAMEBUFFER, "Framebuffer for " + name + " " + String::From((int) i));

                    value.swapchain.Add(framebuffer);
                }
            }

            value.frameUsed = mCurrentFrame;

            BERSERK_VK_LOG_INFO(BERSERK_TEXT("Cache new Framebuffer(s): {0} {1}"), value.framebuffer?value.framebuffer:value.swapchain[0], name);
        }

        void VulkanFramebufferCache::ReleaseFramebuffer(VulkanFramebufferCache::FramebufferValue &value) const {
            BERSERK_VK_LOG_INFO(BERSERK_TEXT("Release Framebuffer(s): {0}"), value.framebuffer?value.framebuffer:value.swapchain[0]);

            for (auto fbo: value.swapchain)
                vkDestroyFramebuffer(mDevice.GetDevice(), fbo, nullptr);
            if (value.framebuffer)
                vkDestroyFramebuffer(mDevice.GetDevice(), value.framebuffer, nullptr);

            value.swapchain.Clear();
            value.framebuffer = nullptr;
            value.frameUsed = 0;
        }
    }
}

