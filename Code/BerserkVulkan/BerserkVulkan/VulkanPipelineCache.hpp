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

#ifndef BERSERK_VULKANPIPELINECACHE_HPP
#define BERSERK_VULKANPIPELINECACHE_HPP

#include <BerserkVulkan/VulkanDefs.hpp>
#include <BerserkRHI/RHIPipelineState.hpp>
#include <BerserkCore/Templates/HashTable.hpp>
#include <BerserkCore/Templates/SmartPointer.hpp>

namespace Berserk {
    namespace RHI {

        /**
         * @brief Vulkan pipelines cache.
         *
         * Caches complete pipelines, which are ready for rendering.
         * Pipeline is built from compiled shader, render pass object,
         * settings, geometry type and etc. All this parameters are used
         * to define, which pipeline to use.
         *
         * Also manages pipeline layouts cache (since almost all pipelines will have its own layout).
         * Pipeline layout is built from vulkan program reflection meta information.
         * Layouts with the same reflection meta can be shared across multiple pipelines.
         *
         * @note Runtime pipeline creations may result in glitches,
         *       but RHI commands are consumed on the separate thread, so
         *       1 - 2 pipelines can be created in the frame if it happens
         *       not so frequently.
         */
        class VulkanPipelineCache {
        public:

            /** Layout is defined by shader program meta info */
            struct PipelineLayoutKey {
                RefCounted<ProgramMeta> meta;
            };

            /** Information, required to allocate descriptor sets for a given pipeline object. */
            struct ResourcesBindingInfo {
                VkDescriptorSetLayout descriptorSetLayout = nullptr;
                uint32 mSamplers = 0;
                uint32 mUniformBLocks = 0;
            };

            /*
             * Pipeline layout (actual vk object plus resource layout
             * (currently we do not support multiple descriptor sets))
             */
            struct PipelineLayoutValue {
                ResourcesBindingInfo resourcesBinding{};
                VkPipelineLayout pipelineLayout = nullptr;
                uint32 frameUsed = 0;
            };

            /*
             * Pipeline state is created from render pass (must be acquired from fbo cache)
             * and from actual pipeline description and primitives to be drawn
             */
            struct PipelineDescriptor {
                PipelineState pipelineState;
                VkRenderPass renderPass = nullptr;
                PrimitivesType primitivesType{};
            };

            struct PipelineKey {
                PipelineState pipelineState;
                VkRenderPass renderPass;
                PrimitivesType primitivesType;
                CrcHash hash;
            }; // sizeof(PipelineKey) ~ 72 bytes

            /* Also stores layout reference in order to allocate descriptor sets for a given pipeline. */
            struct PipelineValue {
                VkPipeline handle;
                PipelineLayoutValue* layout;
                uint32 frameUsed;
            };

            static const uint32 RELEASE_FREQUENCY = 2;
            static const uint32 TIME_TO_KEEP = 4;

            explicit VulkanPipelineCache(class VulkanDevice& device, uint32 releaseFrequency = RELEASE_FREQUENCY, uint32 timeToKeep = TIME_TO_KEEP);
            VulkanPipelineCache(const VulkanPipelineCache&) = delete;
            VulkanPipelineCache(VulkanPipelineCache&&) noexcept = delete;
            ~VulkanPipelineCache();

            /** Attempts to find suitable pipeline, if failed creates new one */
            VkPipeline GetOrCreatePipeline(const PipelineDescriptor& descriptor);

            /** Releases cache accordingly to the settings (must be called every frame) */
            void GC();

        private:
            void CreatePipelineKey(const PipelineDescriptor& descriptor, PipelineKey& key) const;
            void CreatePipelineValue(const PipelineDescriptor& descriptor, PipelineValue& value);
            void ReleasePipeline(PipelineValue& value);

            void CreatePipelineLayoutKey(const RefCounted<ProgramMeta> &meta, PipelineLayoutKey& key) const;
            void CreatePipelineLayoutValue(const RefCounted<ProgramMeta> &meta, PipelineLayoutValue& value) const;
            void ReleasePipelineLayout(PipelineLayoutValue& value);

            PipelineLayoutValue* GetOrCreatePipelineLayout(const RefCounted<ProgramMeta> &meta);

            HashTable<PipelineKey, PipelineValue> mPipelines;
            HashTable<PipelineLayoutKey, PipelineLayoutValue> mLayouts;
            class VulkanDevice& mDevice;
            uint32 mReleaseFrequency;
            uint32 mTimeToKeep;
            uint32 mLastFrameRelease = 0;
            uint32 mCurrentFrame = 0;
        };

    }
}

#endif //BERSERK_VULKANPIPELINECACHE_HPP