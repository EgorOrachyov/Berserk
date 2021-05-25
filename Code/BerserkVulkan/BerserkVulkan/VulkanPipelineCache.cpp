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

#include <BerserkVulkan/VulkanPipelineCache.hpp>
#include <BerserkVulkan/VulkanProgram.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanDebug.hpp>

namespace Berserk {

    template<>
    class Equals<RHI::VulkanPipelineCache::PipelineKey> {
    public:
        bool operator()(const RHI::VulkanPipelineCache::PipelineKey& a, const RHI::VulkanPipelineCache::PipelineKey& b) const {
            auto& ap = a.pipelineState;
            auto& bp = b.pipelineState;

            return a.hash == b.hash &&
                   a.renderPass == b.renderPass &&
                   a.primitivesType == b.primitivesType &&
                   ap.program == bp.program &&
                   ap.declaration == bp.declaration &&
                   ap.rasterState == bp.rasterState &&
                   ap.depthStencilState == bp.depthStencilState &&
                   ap.blendState == bp.blendState;
        }
    };

    template<>
    class Hash<RHI::VulkanPipelineCache::PipelineKey> {
    public:
        uint32 operator()(const RHI::VulkanPipelineCache::PipelineKey& key) const {
            return key.hash;
        }
    };

    template<>
    class Equals<RHI::VulkanPipelineCache::PipelineLayoutKey> {
    public:
        bool operator()(const RHI::VulkanPipelineCache::PipelineLayoutKey& a, const RHI::VulkanPipelineCache::PipelineLayoutKey& b) const {
            return a.meta == b.meta;
        }
    };

    template<>
    class Hash<RHI::VulkanPipelineCache::PipelineLayoutKey> {
    public:
        uint32 operator()(const RHI::VulkanPipelineCache::PipelineLayoutKey& key) const {
            Hash<RefCounted<RHI::ProgramMeta>> hash;
            return hash(key.meta);
        }
    };

    namespace RHI {

        static uint32 HashPipelineKey(const VulkanPipelineCache::PipelineKey& key) {
            Crc32Builder builder;
            builder.Hash(&key, sizeof(VulkanPipelineCache::PipelineKey));
            return builder.GetHash();
        }

        VulkanPipelineCache::VulkanPipelineCache(class VulkanDevice &device, uint32 releaseFrequency, uint32 timeToKeep)
                : mDevice(device), mReleaseFrequency(releaseFrequency), mTimeToKeep(timeToKeep) {

        }

        VulkanPipelineCache::~VulkanPipelineCache() {
            for (auto& entry: mPipelines) {
                auto& value = entry.GetSecond();
                ReleasePipeline(value);
            }
        }

        VkPipeline VulkanPipelineCache::GetOrCreatePipeline(const PipelineDescriptor &descriptor) {
            PipelineKey key{};
            CreatePipelineKey(descriptor, key);

            auto valuePtr = mPipelines.GetPtr(key);

            if (valuePtr != nullptr) {
                // Remember to also update layout usage
                valuePtr->frameUsed = mCurrentFrame;
                valuePtr->layout->frameUsed = mCurrentFrame;
                return valuePtr->handle;
            }

            PipelineValue value{};
            CreatePipelineValue(descriptor, value);

            VkPipeline handle = value.handle;

            mPipelines.Move(key, value);
            return handle;
        }

        void VulkanPipelineCache::GC() {
            if (mCurrentFrame >= mLastFrameRelease + mReleaseFrequency) {
                mLastFrameRelease = mCurrentFrame;

                auto current = mPipelines.begin();
                while (current != mPipelines.end()) {
                    auto& e = *current;

                    if (e.GetSecond().frameUsed + mTimeToKeep <= mCurrentFrame) {
                        ReleasePipeline(e.GetSecond());
                        current = mPipelines.Remove(current);
                    }
                    else
                        ++current;
                }

                auto layout = mLayouts.begin();
                while (layout != mLayouts.end()) {
                    auto& e = *layout;

                    if (e.GetSecond().frameUsed + mTimeToKeep <= mCurrentFrame) {
                        ReleasePipelineLayout(e.GetSecond());
                        layout = mLayouts.Remove(layout);
                    }
                    else
                        ++layout;
                }
            }

            mCurrentFrame += 1;
        }

        void VulkanPipelineCache::CreatePipelineKey(const PipelineDescriptor &descriptor, PipelineKey &key) const {
            Memory::Set(&key, 0x0, sizeof(PipelineKey));

            key.pipelineState = descriptor.pipelineState;
            key.renderPass = descriptor.renderPass;
            key.primitivesType = descriptor.primitivesType;

            key.hash = HashPipelineKey(key);
        }

        void VulkanPipelineCache::CreatePipelineValue(const PipelineDescriptor &descriptor, PipelineValue &value) {
            auto& pipelineState = descriptor.pipelineState;
            auto& rasterState = pipelineState.rasterState;
            auto& blendState = pipelineState.blendState;
            auto& declaration = pipelineState.declaration;
            auto& depthStencilState = pipelineState.depthStencilState;
            auto vkProgram = (const VulkanProgram*) pipelineState.program.Get();
            auto meta = vkProgram->GetProgramMeta();

            assert(declaration);
            assert(vkProgram);
            assert(vkProgram->GetCompilationStatus() == Program::Status::Compiled);
            assert(meta);

            ArrayFixed<VkPipelineShaderStageCreateInfo, Limits::MAX_SHADER_STAGES> shaderStagesCreateInfo;

            for (size_t i = 0; i < vkProgram->GetStages().GetSize(); i++) {
                auto& stage = vkProgram->GetStages()[i];
                auto& module = vkProgram->GetModules()[i];

                VkPipelineShaderStageCreateInfo& stageCreateInfo = shaderStagesCreateInfo.Emplace();
                stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                stageCreateInfo.stage = VulkanDefs::GetShaderType(stage.type);
                stageCreateInfo.module = module;
                stageCreateInfo.pName = "main";
                stageCreateInfo.flags = 0;
                stageCreateInfo.pNext = nullptr;
                stageCreateInfo.pSpecializationInfo = nullptr;
            }

            ArrayFixed<bool, Limits::MAX_VERTEX_BUFFERS> bufferSpecified;
            ArrayFixed<VkVertexInputBindingDescription, Limits::MAX_VERTEX_BUFFERS> buffersDescriptors;
            ArrayFixed<VkVertexInputAttributeDescription, Limits::MAX_VERTEX_ATTRIBUTES> attributesDescriptors;

            bufferSpecified.Resize(Limits::MAX_VERTEX_BUFFERS, false);

            for (size_t i = 0; i < declaration->GetElements().GetSize(); i++) {
                auto& element = declaration->GetElements()[i];

                VkVertexInputAttributeDescription& attributeDescription = attributesDescriptors.Emplace();
                attributeDescription.location = i;
                attributeDescription.offset = element.offset;
                attributeDescription.binding = element.buffer;
                attributeDescription.format = VulkanDefs::GetVertexElementType(element.type);

                if (!bufferSpecified[element.buffer]) {
                    VkVertexInputBindingDescription& bindingDescription = buffersDescriptors.Emplace();
                    bindingDescription.binding = element.buffer;
                    bindingDescription.stride = element.stride;
                    bindingDescription.inputRate = VulkanDefs::GetVertexFrequency(element.frequency);
                }
            }

            VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
            vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            vertexInputStateCreateInfo.vertexBindingDescriptionCount = buffersDescriptors.GetSize();
            vertexInputStateCreateInfo.pVertexBindingDescriptions = buffersDescriptors.GetData();
            vertexInputStateCreateInfo.vertexAttributeDescriptionCount = attributesDescriptors.GetSize();
            vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributesDescriptors.GetData();

            VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
            inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            inputAssembly.topology = VulkanDefs::GetPrimitivesType(descriptor.primitivesType);
            inputAssembly.primitiveRestartEnable = VK_FALSE;

            VkPipelineViewportStateCreateInfo viewportState{};
            viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            viewportState.viewportCount = 1;
            viewportState.scissorCount = 1;

            VkPipelineRasterizationStateCreateInfo rasterizer{};
            rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            rasterizer.depthClampEnable = VK_FALSE;
            rasterizer.rasterizerDiscardEnable = VK_FALSE;
            rasterizer.polygonMode = VulkanDefs::GetPolygonMode(rasterState.mode);
            rasterizer.lineWidth = rasterState.lineWidth;
            rasterizer.cullMode = VulkanDefs::GetPolygonCullMode(rasterState.cullMode);
            rasterizer.frontFace = VulkanDefs::GetPolygonFrontFace(rasterState.frontFace);
            rasterizer.depthBiasEnable = VK_FALSE;
            rasterizer.depthBiasConstantFactor = 0.0f;
            rasterizer.depthBiasClamp = 0.0f;
            rasterizer.depthBiasSlopeFactor = 0.0f;

            VkPipelineMultisampleStateCreateInfo multisampling{};
            multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            multisampling.sampleShadingEnable = VK_FALSE;
            multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
            multisampling.minSampleShading = 1.0f;
            multisampling.pSampleMask = nullptr;
            multisampling.alphaToCoverageEnable = VK_FALSE;
            multisampling.alphaToOneEnable = VK_FALSE;

            VkPipelineDepthStencilStateCreateInfo depthStencil{};
            depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            depthStencil.depthTestEnable = depthStencilState.depthEnable;
            depthStencil.depthWriteEnable = depthStencilState.depthWrite;
            depthStencil.depthCompareOp = VulkanDefs::GetCompareFunction(depthStencilState.compareFunction);
            depthStencil.depthBoundsTestEnable = false;
            depthStencil.minDepthBounds = 0.0f;
            depthStencil.maxDepthBounds = 1.0f;
            depthStencil.stencilTestEnable = depthStencilState.stencilEnable;
            depthStencil.front = VulkanDefs::GetStencilOpState(depthStencilState);
            depthStencil.back = VulkanDefs::GetStencilOpState(depthStencilState);

            ArrayFixed<VkPipelineColorBlendAttachmentState, Limits::MAX_COLOR_ATTACHMENTS> blendAttachments;

            for (auto& config: blendState.attachments) {
                VkPipelineColorBlendAttachmentState& blendAttachment = blendAttachments.Emplace();
                blendAttachment.blendEnable = config.enable;
                blendAttachment.srcColorBlendFactor = VulkanDefs::GetBlendFactor(config.srcColorBlendFactor);
                blendAttachment.dstColorBlendFactor = VulkanDefs::GetBlendFactor(config.dstColorBlendFactor);
                blendAttachment.colorBlendOp = VulkanDefs::GetBlendOperation(config.colorBlendOp);
                blendAttachment.srcAlphaBlendFactor = VulkanDefs::GetBlendFactor(config.srcAlphaBlendFactor);
                blendAttachment.dstAlphaBlendFactor = VulkanDefs::GetBlendFactor(config.dstAlphaBlendFactor);
                blendAttachment.alphaBlendOp = VulkanDefs::GetBlendOperation(config.alphaBlendOp);
                blendAttachment.colorWriteMask =
                        VK_COLOR_COMPONENT_R_BIT |
                        VK_COLOR_COMPONENT_G_BIT |
                        VK_COLOR_COMPONENT_B_BIT |
                        VK_COLOR_COMPONENT_A_BIT;
            }

            VkPipelineColorBlendStateCreateInfo colorBlending{};
            colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            colorBlending.logicOpEnable = VK_FALSE;
            colorBlending.logicOp = VK_LOGIC_OP_COPY;
            colorBlending.attachmentCount = blendAttachments.GetSize();
            colorBlending.pAttachments = blendAttachments.GetData();
            colorBlending.blendConstants[0] = 0.0f;
            colorBlending.blendConstants[1] = 0.0f;
            colorBlending.blendConstants[2] = 0.0f;
            colorBlending.blendConstants[3] = 0.0f;

            VkDynamicState dynamicStates[] = {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR,
            };

            VkPipelineDynamicStateCreateInfo dynamicState{};
            dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
            dynamicState.dynamicStateCount = sizeof(dynamicStates) / sizeof(dynamicStates[0]);
            dynamicState.pDynamicStates = dynamicStates;

            PipelineLayoutValue* layoutObject = GetOrCreatePipelineLayout(meta);

            VkGraphicsPipelineCreateInfo pipelineInfo{};
            pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pipelineInfo.stageCount = shaderStagesCreateInfo.GetSize();
            pipelineInfo.pStages = shaderStagesCreateInfo.GetData();
            pipelineInfo.pVertexInputState = &vertexInputStateCreateInfo;
            pipelineInfo.pInputAssemblyState = &inputAssembly;
            pipelineInfo.pViewportState = &viewportState;
            pipelineInfo.pRasterizationState = &rasterizer;
            pipelineInfo.pMultisampleState = &multisampling;
            pipelineInfo.pDepthStencilState = &depthStencil;
            pipelineInfo.pColorBlendState = &colorBlending;
            pipelineInfo.pDynamicState = &dynamicState;
            pipelineInfo.layout = layoutObject->pipelineLayout;
            pipelineInfo.renderPass = descriptor.renderPass;
            pipelineInfo.subpass = 0;
            pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
            pipelineInfo.basePipelineIndex = -1;

            VkPipeline pipeline;

            BERSERK_VK_CHECK(vkCreateGraphicsPipelines(mDevice.GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline));
            BERSERK_VK_NAME(mDevice.GetDevice(), pipeline, VK_OBJECT_TYPE_PIPELINE, "Pipeline for " + vkProgram->GetShaderName());

            value.handle = pipeline;
            value.layout = layoutObject;
            value.frameUsed = mCurrentFrame;
        }

        void VulkanPipelineCache::ReleasePipeline(PipelineValue &value) {
            vkDestroyPipeline(mDevice.GetDevice(), value.handle, nullptr);
        }

        void VulkanPipelineCache::CreatePipelineLayoutKey(const RefCounted<ProgramMeta> &meta, PipelineLayoutKey &key) const {
            key.meta = meta;
        }

        void VulkanPipelineCache::CreatePipelineLayoutValue(const RefCounted<ProgramMeta> &meta, PipelineLayoutValue &value) const {
            assert(meta);

            Array<VkDescriptorSetLayoutBinding> bindings;

            auto& samplers = meta->samplers;
            auto samplersCount = samplers.GetSize();

            auto& uniformBlocks = meta->paramBlocks;
            auto uniformBlocksCount = uniformBlocks.GetSize();

            bindings.EnsureToAdd(samplersCount + uniformBlocksCount);

            for (auto& entry: samplers) {
                auto& sampler = entry.GetSecond();

                assert(sampler.arraySize >= 1);

                VkDescriptorSetLayoutBinding& binding = bindings.Emplace();
                binding.binding = sampler.location;
                binding.descriptorCount = sampler.arraySize;
                binding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
                binding.stageFlags = VK_SHADER_STAGE_ALL;
                binding.pImmutableSamplers = nullptr;
            }

            for (auto& entry: uniformBlocks) {
                auto& block = entry.GetSecond();

                VkDescriptorSetLayoutBinding& binding = bindings.Emplace();
                binding.binding = block.slot;
                binding.descriptorCount = 1;
                binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                binding.stageFlags = VK_SHADER_STAGE_ALL;
                binding.pImmutableSamplers = nullptr;
            }

            VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
            descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            descriptorSetLayoutInfo.bindingCount = bindings.GetSize();
            descriptorSetLayoutInfo.pBindings = bindings.GetData();

            VkDescriptorSetLayout descriptorSetLayout;
            BERSERK_VK_CHECK(vkCreateDescriptorSetLayout(mDevice.GetDevice(), &descriptorSetLayoutInfo, nullptr, &descriptorSetLayout));
            BERSERK_VK_NAME(mDevice.GetDevice(), descriptorSetLayout, VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, "Desc set layout for " + meta->name);

            VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
            pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutCreateInfo.setLayoutCount = 1;
            pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;
            pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
            pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

            VkPipelineLayout pipelineLayout;
            BERSERK_VK_CHECK(vkCreatePipelineLayout(mDevice.GetDevice(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout));
            BERSERK_VK_NAME(mDevice.GetDevice(), descriptorSetLayout, VK_OBJECT_TYPE_PIPELINE_LAYOUT, "Pipeline layout for " + meta->name);

            ResourcesBindingInfo bindingInfo;
            bindingInfo.descriptorSetLayout = descriptorSetLayout;
            bindingInfo.mSamplers = samplersCount;
            bindingInfo.mUniformBLocks = uniformBlocksCount;

            value.resourcesBinding = bindingInfo;
            value.pipelineLayout = pipelineLayout;
            value.frameUsed = mCurrentFrame;
        }

        void VulkanPipelineCache::ReleasePipelineLayout(PipelineLayoutValue &value) {
            vkDestroyPipelineLayout(mDevice.GetDevice(), value.pipelineLayout, nullptr);
            vkDestroyDescriptorSetLayout(mDevice.mDevice, value.resourcesBinding.descriptorSetLayout, nullptr);
        }

        VulkanPipelineCache::PipelineLayoutValue* VulkanPipelineCache::GetOrCreatePipelineLayout(const RefCounted<ProgramMeta> &meta) {
            PipelineLayoutKey key;
            CreatePipelineLayoutKey(meta, key);

            auto valuePtr = mLayouts.GetPtr(key);

            if (valuePtr != nullptr) {
                auto& layout = *valuePtr;

                layout.frameUsed = mCurrentFrame;
                return valuePtr;
            }

            PipelineLayoutValue value;
            CreatePipelineLayoutValue(meta, value);

            mLayouts.Add(key, value);
            return mLayouts.GetPtr(key);
        }

    }
}