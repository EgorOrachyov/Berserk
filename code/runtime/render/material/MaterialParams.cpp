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

#include <core/Engine.hpp>
#include <render/material/Material.hpp>
#include <render/material/MaterialParams.hpp>

BRK_NS_BEGIN

MaterialParams::MaterialParams(class Material &material) {
    const auto &technique = material.GetTechnique();
    const auto &passes = technique->GetPasses();

    mResourceSets.reserve(passes.size());
    mUniformBuffers.reserve(passes.size());

    auto &device = Engine::Instance().GetRHIDevice();
    static StringName nShaderParams(SHADER_PARAMS_BLOCK);

    for (auto &pass : passes) {
        const auto &passProgram = pass->GetShader();
        const auto meta = passProgram->GetShaderMeta();

        auto blockInfo = meta->paramBlocks.find(nShaderParams);
        if (blockInfo == meta->paramBlocks.end()) {
            BRK_ERROR("No block=" << nShaderParams << " in pass name=" << pass->GetName());
            continue;
        }

        RHIBufferDesc bufferDesc{};
        bufferDesc.size = blockInfo->second.size;
        bufferDesc.bufferUsage = RHIBufferUsage::Dynamic;

        RHIResourceSetDesc setDesc{};

        mUniformBuffers.push_back(device.CreateUniformBuffer(bufferDesc));
        mResourceSets.push_back(device.CreateResourceSet(setDesc));
    }

    Update(material);
}

void MaterialParams::Update(class Material &material) {
    const auto &shader = material.GetShader();
    const auto &technique = material.GetTechnique();
    const auto &passes = technique->GetPasses();
    const auto &params = shader->GetParams();

    assert(mResourceSets.size() == passes.size());

    auto &device = Engine::Instance().GetRHIDevice();
    static StringName nShaderParams(SHADER_PARAMS_BLOCK);

    // NOTE: in future maybe we need to track modified resources and values and
    // update only them (use history mark or track modified by boolean flags).
    // For know all resources are update and rewritten

    for (size_t passIdx = 0; passIdx < passes.size(); passIdx++) {
        const auto &pass = passes[passIdx];
        const auto &program = pass->GetShader();
        const auto meta = program->GetShaderMeta();

        const auto &samplers = material.GetSamplers();
        const auto &textures = material.GetTextures();
        const auto &data = material.GetDataParams();

        auto dataBuffer = Data::Make(mUniformBuffers[passIdx]->GetSize());
        Memory::Set(dataBuffer->GetDataWrite(), 0x0, dataBuffer->GetSize());

        RHIResourceSetDesc resourceSetDesc;

        for (const auto &paramNameId : params->GetParamLookUp()) {
            const auto &name = paramNameId.first;
            const auto id = paramNameId.second;
            const auto &param = params->GetParamsLight()[id];

            if (param.type == ShaderParamType::Texture) {
                auto metaQuery = meta->samplers.find(name);
                if (metaQuery != meta->samplers.end()) {
                    // If this texture param is present in the pass shader
                    // then set all array textures and samplers of this param
                    auto baseLocation = metaQuery->second.location;
                    auto baseOffset = params->GetTextureParamsInfo()[param.info].offset;
                    for (uint32 i = 0; i < param.arraySize; i++) {
                        // Skip null textures
                        if (textures[baseOffset + i].IsNotNull()) {
                            resourceSetDesc.AddSampler(samplers[baseOffset + i], baseLocation, i);
                            resourceSetDesc.AddTexture(textures[baseOffset + i], baseLocation, i);
                        }
                    }
                }
            } else if (param.type == ShaderParamType::Data) {
                auto metaQuery = meta->params.find(name);
                if (metaQuery != meta->params.end()) {
                    // If data param is present, then pack it all array elements
                    // and transform data if required for the layout
                    const auto typeData = param.typeData;
                    const auto srcSize = params->GetDataParamsInfo()[param.info].size;
                    const auto srcOffset = params->GetDataParamsInfo()[param.info].offset;
                    const auto dstOffset = metaQuery->second.blockOffset;
                    const auto dstArrayStride = metaQuery->second.arrayStride;
                    const auto dstMatrixStride = metaQuery->second.matrixStride;

                    for (uint32 i = 0; i < param.arraySize; i++) {
                        const unsigned char *src = data.data() + srcOffset + i * srcSize;
                        unsigned char *dst = reinterpret_cast<unsigned char *>(dataBuffer->GetDataWrite()) + dstOffset + i * dstArrayStride;

                        switch (typeData) {
                            case RHIShaderDataType::Mat2: {
                                using MatT = Mat2x2f;
                                MatT mat(reinterpret_cast<const float *>(src), MatT::GetSize());
                                auto matT = mat.Transpose();
                                for (uint32 row = 0; row < MatT::GetDimM(); row++)
                                    Memory::Copy(dst + row * dstMatrixStride,
                                                 matT.GetData() + row * MatT::GetDimN(),
                                                 sizeof(float) * MatT::GetDimN());
                                break;
                            }
                            case RHIShaderDataType::Mat3: {
                                using MatT = Mat3x3f;
                                MatT mat(reinterpret_cast<const float *>(src), MatT::GetSize());
                                auto matT = mat.Transpose();
                                for (uint32 row = 0; row < MatT::GetDimM(); row++)
                                    Memory::Copy(dst + row * dstMatrixStride,
                                                 matT.GetData() + row * MatT::GetDimN(),
                                                 sizeof(float) * MatT::GetDimN());
                                break;
                            }
                            case RHIShaderDataType::Mat4: {
                                using MatT = Mat4x4f;
                                MatT mat(reinterpret_cast<const float *>(src), MatT::GetSize());
                                auto matT = mat.Transpose();
                                for (uint32 row = 0; row < MatT::GetDimM(); row++)
                                    Memory::Copy(dst + row * dstMatrixStride,
                                                 matT.GetData() + row * MatT::GetDimN(),
                                                 sizeof(float) * MatT::GetDimN());
                                break;
                            }
                            default:
                                Memory::Copy(dst, src, srcSize);
                                break;
                        }
                    }
                }
            }
        }

        if (mUniformBuffers[passIdx].IsNotNull()) {
            // Update uniform buffer
            device.UpdateUniformBuffer(mUniformBuffers[passIdx], 0, dataBuffer->GetSize(), dataBuffer);

            // Add info about uniform buffer of the resource set for the pass
            auto blockInfo = meta->paramBlocks.find(nShaderParams);
            assert(blockInfo != meta->paramBlocks.end());

            resourceSetDesc.AddBuffer(mUniformBuffers[passIdx], blockInfo->second.slot, 0, blockInfo->second.size);
        }

        // Update resource set
        device.UpdateResourceSet(mResourceSets[passIdx], resourceSetDesc);
    }
}

BRK_NS_END