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

#include <BerserkVulkan/VulkanProgramCompiler.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanProgram.hpp>
#include <glslang/Public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>

namespace Berserk {
    namespace RHI {

        EShLanguage GetShaderType(ShaderType type) {
            switch (type) {
                case ShaderType::Vertex:
                    return EShLangVertex;
                case ShaderType::Fragment:
                    return EShLangFragment;
                default:
                    BERSERK_VK_LOG_ERROR("Unsupported ShaderType");
                    return EShLangCount;
            }
        }

        VulkanProgramCompiler::VulkanProgramCompiler(class VulkanDevice& device) : mDevice(device) {
            auto status = glslang::InitializeProcess();

            if (!status) {
                BERSERK_VK_LOG_ERROR("Failed to initialize glslang");
                BERSERK_EXCEPT(FatalError, "Failed to initialize glslang");
            }
        }

        VulkanProgramCompiler::~VulkanProgramCompiler() {
            glslang::FinalizeProcess();
        }

        RefCounted<Program> VulkanProgramCompiler::CreateProgram(const Program::Desc &desc) {
            auto program = Memory::Make<VulkanProgram>(mDevice, desc);

            ProgramCompileData compileData;
            compileData.program = RefCounted<VulkanProgram>(program);

            // todo: run async task
            Compile(compileData);

            if (compileData.compiled) {
                program->InitializedFromBinary(compileData.binaries, std::move(compileData.meta));
            } else {
                program->NotifyFailedCompile(std::move(compileData.message));
            }

            return RefCounted<Program>(program);
        }

        void VulkanProgramCompiler::Update() {
            // todo: traverse async markers and check if program is compiled
        }

        void VulkanProgramCompiler::Compile(ProgramCompileData &compileData) const {
            assert(compileData.program);
            assert(ValidateStages(*compileData.program));
            assert(mDevice.GetSupportedShaderLanguages().Contains(compileData.program->GetLanguage()));

            glslang::TProgram program;
            ArrayFixed<glslang::TShader, Limits::MAX_SHADER_STAGES> shaders;
            ArrayFixed<EShLanguage, Limits::MAX_SHADER_STAGES> stagesTypes;

            auto messages = (EShMessages) (EShMsgSpvRules | EShMsgVulkanRules);

            for (auto& stage: compileData.program->GetStages()) {
                assert(stage.sourceCode);

                EShLanguage& language = stagesTypes.Emplace(GetShaderType(stage.type));
                glslang::TShader& shader = shaders.Emplace(language);

                const char* sources[1] = { (const char*) stage.sourceCode->GetData() };
                const int lengths[1] = { (int) stage.sourceCode->GetSize() };

                shader.setStringsWithLengths(sources, lengths, 1);

                int version = 100;
                glslang::EShClient client = glslang::EShClientVulkan;
                glslang::EShTargetClientVersion clientVersion = glslang::EShTargetVulkan_1_0;
                glslang::EShTargetLanguageVersion targetVersion = glslang::EShTargetSpv_1_0;

                shader.setEnvInput(glslang::EShSourceGlsl, language, client, version);
                shader.setEnvClient(glslang::EShClientVulkan, clientVersion);
                shader.setEnvTarget(glslang::EShTargetSpv, targetVersion);
                shader.setEntryPoint("main");

                TBuiltInResource defaultTBuiltInResource{};
                defaultTBuiltInResource.maxLights = 32;
                defaultTBuiltInResource.maxClipPlanes = 6;
                defaultTBuiltInResource.maxTextureUnits = 32;
                defaultTBuiltInResource.maxTextureCoords = 32;
                defaultTBuiltInResource.maxVertexAttribs = 64;
                defaultTBuiltInResource.maxVertexUniformComponents = 4096;
                defaultTBuiltInResource.maxVaryingFloats = 64;
                defaultTBuiltInResource.maxVertexTextureImageUnits = 32;
                defaultTBuiltInResource.maxCombinedTextureImageUnits = 80;
                defaultTBuiltInResource.maxTextureImageUnits = 32;
                defaultTBuiltInResource.maxFragmentUniformComponents = 4096;
                defaultTBuiltInResource.maxDrawBuffers = 32;
                defaultTBuiltInResource.maxVertexUniformVectors = 128;
                defaultTBuiltInResource.maxVaryingVectors = 8;
                defaultTBuiltInResource.maxFragmentUniformVectors = 16;
                defaultTBuiltInResource.maxVertexOutputVectors = 16;
                defaultTBuiltInResource.maxFragmentInputVectors = 15;
                defaultTBuiltInResource.minProgramTexelOffset = -8;
                defaultTBuiltInResource.maxProgramTexelOffset = 7;
                defaultTBuiltInResource.maxClipDistances = 8;
                defaultTBuiltInResource.maxComputeWorkGroupCountX = 65535;
                defaultTBuiltInResource.maxComputeWorkGroupCountY = 65535;
                defaultTBuiltInResource.maxComputeWorkGroupCountZ = 65535;
                defaultTBuiltInResource.maxComputeWorkGroupSizeX = 1024;
                defaultTBuiltInResource.maxComputeWorkGroupSizeY = 1024;
                defaultTBuiltInResource.maxComputeWorkGroupSizeZ = 64;
                defaultTBuiltInResource.maxComputeUniformComponents = 1024;
                defaultTBuiltInResource.maxComputeTextureImageUnits = 16;
                defaultTBuiltInResource.maxComputeImageUniforms = 8;
                defaultTBuiltInResource.maxComputeAtomicCounters = 8;
                defaultTBuiltInResource.maxComputeAtomicCounterBuffers = 1;
                defaultTBuiltInResource.maxVaryingComponents = 60;
                defaultTBuiltInResource.maxVertexOutputComponents = 64;
                defaultTBuiltInResource.maxGeometryInputComponents = 64;
                defaultTBuiltInResource.maxGeometryOutputComponents = 128;
                defaultTBuiltInResource.maxFragmentInputComponents = 128;
                defaultTBuiltInResource.maxImageUnits = 8;
                defaultTBuiltInResource.maxCombinedImageUnitsAndFragmentOutputs = 8;
                defaultTBuiltInResource.maxCombinedShaderOutputResources = 8;
                defaultTBuiltInResource.maxImageSamples = 0;
                defaultTBuiltInResource.maxVertexImageUniforms = 0;
                defaultTBuiltInResource.maxTessControlImageUniforms = 0;
                defaultTBuiltInResource.maxTessEvaluationImageUniforms = 0;
                defaultTBuiltInResource.maxGeometryImageUniforms = 0;
                defaultTBuiltInResource.maxFragmentImageUniforms = 8;
                defaultTBuiltInResource.maxCombinedImageUniforms = 8;
                defaultTBuiltInResource.maxGeometryTextureImageUnits = 16;
                defaultTBuiltInResource.maxGeometryOutputVertices = 256;
                defaultTBuiltInResource.maxGeometryTotalOutputComponents = 1024;
                defaultTBuiltInResource.maxGeometryUniformComponents = 1024;
                defaultTBuiltInResource.maxGeometryVaryingComponents = 64;
                defaultTBuiltInResource.maxTessControlInputComponents = 128;
                defaultTBuiltInResource.maxTessControlOutputComponents = 128;
                defaultTBuiltInResource.maxTessControlTextureImageUnits = 16;
                defaultTBuiltInResource.maxTessControlUniformComponents = 1024;
                defaultTBuiltInResource.maxTessControlTotalOutputComponents = 4096;
                defaultTBuiltInResource.maxTessEvaluationInputComponents = 128;
                defaultTBuiltInResource.maxTessEvaluationOutputComponents = 128;
                defaultTBuiltInResource.maxTessEvaluationTextureImageUnits = 16;
                defaultTBuiltInResource.maxTessEvaluationUniformComponents = 1024;
                defaultTBuiltInResource.maxTessPatchComponents = 120;
                defaultTBuiltInResource.maxPatchVertices = 32;
                defaultTBuiltInResource.maxTessGenLevel = 64;
                defaultTBuiltInResource.maxViewports = 16;
                defaultTBuiltInResource.maxVertexAtomicCounters = 0;
                defaultTBuiltInResource.maxTessControlAtomicCounters = 0;
                defaultTBuiltInResource.maxTessEvaluationAtomicCounters = 0;
                defaultTBuiltInResource.maxGeometryAtomicCounters = 0;
                defaultTBuiltInResource.maxFragmentAtomicCounters = 8;
                defaultTBuiltInResource.maxCombinedAtomicCounters = 8;
                defaultTBuiltInResource.maxAtomicCounterBindings = 1;
                defaultTBuiltInResource.maxVertexAtomicCounterBuffers = 0;
                defaultTBuiltInResource.maxTessControlAtomicCounterBuffers = 0;
                defaultTBuiltInResource.maxTessEvaluationAtomicCounterBuffers = 0;
                defaultTBuiltInResource.maxGeometryAtomicCounterBuffers = 0;
                defaultTBuiltInResource.maxFragmentAtomicCounterBuffers = 1;
                defaultTBuiltInResource.maxCombinedAtomicCounterBuffers = 1;
                defaultTBuiltInResource.maxAtomicCounterBufferSize = 16384;
                defaultTBuiltInResource.maxTransformFeedbackBuffers = 4;
                defaultTBuiltInResource.maxTransformFeedbackInterleavedComponents = 64;
                defaultTBuiltInResource.maxCullDistances = 8;
                defaultTBuiltInResource.maxCombinedClipAndCullDistances = 8;
                defaultTBuiltInResource.maxSamples = 4;
                defaultTBuiltInResource.maxMeshOutputVerticesNV = 256;
                defaultTBuiltInResource.maxMeshOutputPrimitivesNV = 512;
                defaultTBuiltInResource.maxMeshWorkGroupSizeX_NV = 32;
                defaultTBuiltInResource.maxMeshWorkGroupSizeY_NV = 1;
                defaultTBuiltInResource.maxMeshWorkGroupSizeZ_NV = 1;
                defaultTBuiltInResource.maxTaskWorkGroupSizeX_NV = 32;
                defaultTBuiltInResource.maxTaskWorkGroupSizeY_NV = 1;
                defaultTBuiltInResource.maxTaskWorkGroupSizeZ_NV = 1;
                defaultTBuiltInResource.maxMeshViewCountNV = 4;
                defaultTBuiltInResource.maxDualSourceDrawBuffersEXT = 1;

                defaultTBuiltInResource.limits.nonInductiveForLoops = true;
                defaultTBuiltInResource.limits.whileLoops = true;
                defaultTBuiltInResource.limits.doWhileLoops = true;
                defaultTBuiltInResource.limits.generalUniformIndexing = true;
                defaultTBuiltInResource.limits.generalAttributeMatrixVectorIndexing = true;
                defaultTBuiltInResource.limits.generalVaryingIndexing = true;
                defaultTBuiltInResource.limits.generalSamplerIndexing = true;
                defaultTBuiltInResource.limits.generalVariableIndexing = true;
                defaultTBuiltInResource.limits.generalConstantMatrixVectorIndexing = true;

                const int defaultVersion = 100;

                if (!shader.parse(&defaultTBuiltInResource, defaultVersion, true, messages)) {
                    compileData.compiled = false;
                    compileData.message = shader.getInfoLog();
                    return;
                }

                program.addShader(&shader);
            }

            if (!program.link(messages)) {
                compileData.compiled = false;
                compileData.message = program.getInfoLog();
                return;
            }

            for (size_t i = 0; i < stagesTypes.GetSize(); i++) {
                glslang::TIntermediate* stageIntermediate = program.getIntermediate(stagesTypes[i]);
                assert(stageIntermediate);

                std::vector<uint32> bytecode;
                spv::SpvBuildLogger logger;
                glslang::SpvOptions spvOptions;
                spvOptions.disableOptimizer = false;
                spvOptions.validate = true;
                spvOptions.optimizeSize = true;

                glslang::GlslangToSpv(*stageIntermediate, bytecode, &logger, &spvOptions);

                if (bytecode.empty()) {
                    compileData.compiled = false;
                    compileData.message = logger.getAllMessages().c_str();
                    return;
                }

                auto compiledSpirV = SystemMemoryBuffer::Create(bytecode.size() * sizeof(uint32), bytecode.data());
                compileData.binaries.Emplace(compiledSpirV);
            }

            {
                auto result = program.buildReflection();

                if (!result) {
                    compileData.compiled = false;
                    compileData.message = "Failed to build reflection";
                }

                /** Actual meta class, nothing special */
                class VulkanProgramMeta final: public ProgramMeta {
                public:
                    ~VulkanProgramMeta() override = default;
                protected:
                    void OnReleased() const override {
                        Memory::Release(this);
                    }
                };

                /** Allocate program meta */
                RefCounted<ProgramMeta> meta(Memory::Make<VulkanProgramMeta>());

                /** Reflect uniforms */

                auto uniformsCount = program.getNumUniformVariables();
                for (int i = 0; i < uniformsCount; i++) {
                    auto& uniform = program.getUniform(i);

                    auto binding = uniform.getBinding();
                    auto type = uniform.getType();
                    auto basicType = type->getBasicType();

                    if (binding >= 0) {
                        ProgramMeta::ObjectParam objectParam;
                        objectParam.name = uniform.name.c_str();

                        switch (basicType) {
                            case glslang::EbtSampler: {
                                auto sampler = type->getSampler();

                                switch (sampler.dim) {
                                    case glslang::Esd2D:
                                        objectParam.type = sampler.arrayed? ShaderParamType::Sampler2dArray: ShaderParamType::Sampler2d;
                                        break;
                                    case glslang::EsdCube:
                                        objectParam.type = ShaderParamType::SamplerCube;
                                        break;
                                    case glslang::Esd3D:
                                        objectParam.type = ShaderParamType::Sampler3d;
                                        break;
                                    default:
                                        BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Unsupported shader param"));
                                        return;
                                }

                                break;
                            }
                            default:
                                BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Unsupported shader param"));
                                return;
                        }

                        objectParam.location = uniform.getBinding();
                        objectParam.arraySize = uniform.size;

                        meta->samplers.Add(objectParam.name, objectParam);
                    } else {
                        ProgramMeta::DataParam dataParam;
                        dataParam.name = uniform.name.c_str();
                        dataParam.blockIndex = uniform.index;
                        dataParam.blockOffset = uniform.offset;
                        dataParam.arraySize = uniform.size;
                        dataParam.arrayStride = uniform.arrayStride;

                        meta->params.Add(dataParam.name, dataParam);
                    }
                }

                compileData.meta = meta;
            }

            compileData.compiled = true;
        }

        bool VulkanProgramCompiler::ValidateStages(const class VulkanProgram &program) {
            const auto& stages = program.GetStages();
            assert(stages.GetSize() == 2);

            bool vertex = false;
            bool fragment = false;

            for (auto& stage: stages) {
                if (stage.type == ShaderType::Vertex)
                    vertex = true;
                else if (stage.type == ShaderType::Fragment)
                    fragment = true;
            }

            return vertex && fragment;
        }

    }
}