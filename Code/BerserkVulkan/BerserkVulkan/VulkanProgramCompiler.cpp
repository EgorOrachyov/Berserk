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
#include <BerserkCore/Templates/Timer.hpp>
#include <glslang/Public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>


namespace Berserk {
    namespace RHI {

        using ns = std::chrono::nanoseconds;
        using clock = std::chrono::steady_clock;

        static bool ValidateStages(const class VulkanProgram &program) {
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

        static EShLanguage GetShaderType(ShaderType type) {
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

        static VertexElementType GetVertexElementType(glslang::TBasicType basicType, int size) {
            switch (basicType) {
                case glslang::EbtFloat:
                    switch (size) {
                        case 1:
                            return VertexElementType::Float1;
                        case 2:
                            return VertexElementType::Float2;
                        case 3:
                            return VertexElementType::Float3;
                        case 4:
                            return VertexElementType::Float4;
                        default:
                            break;
                    }
                    break;
                case glslang::EbtInt:
                    switch (size) {
                        case 1:
                            return VertexElementType::Int1;
                        case 2:
                            return VertexElementType::Int2;
                        case 3:
                            return VertexElementType::Int3;
                        case 4:
                            return VertexElementType::Int4;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }

            BERSERK_VK_LOG_ERROR("Unsupported VertexElementType");
            return VertexElementType::Unknown;
        }

        static ShaderDataType GetVectorType(glslang::TBasicType basicType, int vecSize) {
            switch (basicType) {
                case glslang::EbtFloat:
                    switch (vecSize) {
                        case 1:
                            return ShaderDataType::Float1;
                        case 2:
                            return ShaderDataType::Float2;
                        case 3:
                            return ShaderDataType::Float3;
                        case 4:
                            return ShaderDataType::Float4;
                        default:
                            break;
                    }
                    break;
                case glslang::EbtInt:
                    switch (vecSize) {
                        case 1:
                            return ShaderDataType::Int1;
                        case 2:
                            return ShaderDataType::Int2;
                        case 3:
                            return ShaderDataType::Int3;
                        case 4:
                            return ShaderDataType::Int4;
                        default:
                            break;
                    }
                    break;
                case glslang::EbtUint:
                    switch (vecSize) {
                        case 1:
                            return ShaderDataType::Uint1;
                        case 2:
                            return ShaderDataType::Uint2;
                        case 3:
                            return ShaderDataType::Uint3;
                        case 4:
                            return ShaderDataType::Uint4;
                        default:
                            break;
                    }
                    break;
                case glslang::EbtBool:
                    switch (vecSize) {
                        case 1:
                            return ShaderDataType::Bool1;
                        case 2:
                            return ShaderDataType::Bool2;
                        case 3:
                            return ShaderDataType::Bool3;
                        case 4:
                            return ShaderDataType::Bool4;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }

            BERSERK_VK_LOG_ERROR("Unsupported ShaderDataType");
            return ShaderDataType::Unknown;
        }

        static ShaderDataType GetMatrixType(glslang::TBasicType basicType, int rows, int cols) {
            switch (basicType) {
                case glslang::EbtFloat:
                    switch (rows) {
                        case 2:
                            if (cols == rows)
                                return ShaderDataType::Mat2;
                            break;
                        case 3:
                            if (cols == rows)
                                return ShaderDataType::Mat3;
                            break;
                        case 4:
                            if (cols == rows)
                                return ShaderDataType::Mat4;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }

            BERSERK_VK_LOG_ERROR("Unsupported ShaderDataType");
            return ShaderDataType::Unknown;
        }

        static ShaderDataType GetShaderDataType(glslang::TBasicType basicType, int vecSize, int rows, int cols) {
            if (vecSize > 0)
                return GetVectorType(basicType, vecSize);
            else
                return GetMatrixType(basicType, rows, cols);
        }

        static ShaderParamType GetSamplerType(glslang::TSamplerDim dim, int arrayed) {
            switch (dim) {
                case glslang::Esd2D:
                    return arrayed? ShaderParamType::Sampler2dArray: ShaderParamType::Sampler2d;
                case glslang::EsdCube:
                    return ShaderParamType::SamplerCube;
                case glslang::Esd3D:
                    return ShaderParamType::Sampler3d;
                default:
                    BERSERK_VK_LOG_ERROR("Unsupported ShaderParamType");
                    return ShaderParamType::Unknown;
            }
        }

        static uint32 GetDataParamSize(ShaderDataType type) {
            switch (type) {
                case ShaderDataType::Float1:
                    return sizeof(float) * 1;
                case ShaderDataType::Float2:
                    return sizeof(float) * 2;
                case ShaderDataType::Float3:
                    return sizeof(float) * 3;
                case ShaderDataType::Float4:
                    return sizeof(float) * 4;
                case ShaderDataType::Int1:
                    return sizeof(int32) * 1;
                case ShaderDataType::Int2:
                    return sizeof(int32) * 2;
                case ShaderDataType::Int3:
                    return sizeof(int32) * 3;
                case ShaderDataType::Int4:
                    return sizeof(int32) * 4;
                case ShaderDataType::Uint1:
                    return sizeof(uint32) * 1;
                case ShaderDataType::Uint2:
                    return sizeof(uint32) * 2;
                case ShaderDataType::Uint3:
                    return sizeof(uint32) * 3;
                case ShaderDataType::Uint4:
                    return sizeof(uint32) * 4;
                case ShaderDataType::Bool1:
                    return sizeof(uint32) * 1;
                case ShaderDataType::Bool2:
                    return sizeof(uint32) * 2;
                case ShaderDataType::Bool3:
                    return sizeof(uint32) * 3;
                case ShaderDataType::Bool4:
                    return sizeof(uint32) * 4;
                case ShaderDataType::Mat2:
                    return sizeof(float) * 2 * 2;
                case ShaderDataType::Mat3:
                    return sizeof(float) * 3 * 3;
                case ShaderDataType::Mat4:
                    return sizeof(float) * 4 * 4;
                default:
                    return 0;
            }
        }

        static bool BuildReflection(glslang::TProgram &program, VulkanProgramCompiler::ProgramCompileData& compileData) {
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

            /** Meta name is the same as program has */
            meta->name = compileData.program->GetShaderName();

            /** Reflect shader program */
            auto inputsCount = program.getNumPipeInputs();

            for (int i = 0; i < inputsCount; i++) {
                auto& input = program.getPipeInput(i);
                auto type = input.getType();

                ProgramMeta::InputAttribute attribute;
                attribute.name = input.name.c_str();
                attribute.location = type->getQualifier().layoutLocation;
                attribute.type = GetVertexElementType(type->getBasicType(), type->getVectorSize());

                assert(attribute.type != VertexElementType::Unknown);

                meta->inputs.Add(attribute.name, attribute);
            }

            /** Reflect uniform blocks */
            auto uniformBlocksCount = program.getNumUniformBlocks();

            Array<uint32> indexToSlot;
            indexToSlot.EnsureToAdd(uniformBlocksCount);

            for (int i = 0; i < uniformBlocksCount; i++) {
                auto& block = program.getUniformBlock(i);

                ProgramMeta::DataParamBlock paramBlock;
                paramBlock.name = block.name.c_str();
                paramBlock.size = block.size;
                paramBlock.slot = block.getBinding();

                indexToSlot.Add(paramBlock.slot);

                meta->paramBlocks.Add(paramBlock.name, paramBlock);
            }

            /** Reflect uniforms */
            auto uniformsCount = program.getNumUniformVariables();

            for (int i = 0; i < uniformsCount; i++) {
                auto& uniform = program.getUniform(i);

                auto binding = uniform.getBinding();
                auto type = uniform.getType();
                auto basicType = type->getBasicType();

                /** Params outside */
                if (binding >= 0) {
                    ProgramMeta::ObjectParam objectParam;
                    objectParam.name = uniform.name.c_str();

                    if (basicType == glslang::EbtSampler) {
                        auto sampler = type->getSampler();
                        objectParam.type = GetSamplerType(sampler.dim, sampler.arrayed);

                        assert(objectParam.type != ShaderParamType::Unknown);
                    } else {
                        compileData.compiled = false;
                        return false;
                    }

                    objectParam.location = binding;
                    objectParam.arraySize = uniform.size;

                    meta->samplers.Add(objectParam.name, objectParam);
                }
                /** Data params */
                else {
                    ProgramMeta::DataParam dataParam;
                    dataParam.name = uniform.name.c_str();
                    dataParam.arraySize = uniform.size;
                    dataParam.arrayStride = uniform.arrayStride;
                    dataParam.blockSlot = indexToSlot[uniform.index];
                    dataParam.blockOffset = uniform.offset;
                    dataParam.type = GetShaderDataType(type->getBasicType(), type->getVectorSize(), type->getMatrixRows(), type->getMatrixCols());
                    dataParam.elementSize = GetDataParamSize(dataParam.type);

                    meta->params.Add(dataParam.name, dataParam);
                }
            }

            compileData.meta = meta;
            return true;
        }

        VulkanProgramCompiler::VulkanProgramCompiler(class VulkanDevice& device) : mDevice(device) {
            auto status = glslang::InitializeProcess();

            if (!status) {
                BERSERK_VK_LOG_ERROR("Failed to initialize glslang");
                BERSERK_EXCEPT(FatalError, "Failed to initialize glslang");
            }
        }

        VulkanProgramCompiler::~VulkanProgramCompiler() {
            // Wait for all async compiled tasks
            while (mToCompile.load() > 0);

            glslang::FinalizeProcess();
        }

        RefCounted<Program> VulkanProgramCompiler::CreateProgram(const Program::Desc &desc) {
            auto program = Memory::Make<VulkanProgram>(mDevice, desc);

            auto compileData = SharedPtr<ProgramCompileData>::Make();
            compileData->program = RefCounted<VulkanProgram>(program);

            // To track num of async compiled task
            mToCompile.fetch_add(1);

            // Submit actual task
            TaskManager::SubmitTask(BERSERK_TEXT("Vk-Compile-") + desc.name, TaskPriority::Medium, [=](TaskContext&){
                Compile(*compileData);
                {
                    // Add data as compiled, so it will be traversed later and finally initialized
                    Guard<Mutex> guard(mMutex);
                    mPendingCreate.Add(compileData);
                }
            });

            return RefCounted<Program>(program);
        }

        void VulkanProgramCompiler::Update() {
            {
                Guard<Mutex> guard(mMutex);
                mToProcessInit = std::move(mPendingCreate);
            }

            for (auto& compileData: mToProcessInit) {
                auto program = compileData->program;
                auto compiled = compileData->compiled;

                if (compiled) {
                    program->InitializedFromBinary(compileData->binaries, std::move(compileData->meta));
                } else {
                    program->NotifyFailedCompile(std::move(compileData->message));
                }
            }

            mToCompile.fetch_sub((uint32) mToProcessInit.GetSize());
            mToProcessInit.Clear();
        }

        void VulkanProgramCompiler::Compile(ProgramCompileData &compileData) const {
            ScopedTimer scopedTimer;

            assert(compileData.program);
            assert(ValidateStages(*compileData.program));
            assert(mDevice.GetSupportedShaderLanguages().Contains(compileData.program->GetLanguage()));

            compileData.compiled = false;

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
                    compileData.message = shader.getInfoLog();
                    return;
                }

                program.addShader(&shader);
            }

            if (!program.link(messages)) {
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
                    compileData.message = logger.getAllMessages().c_str();
                    return;
                }

                auto compiledSpirV = SystemMemoryBuffer::Create(bytecode.size() * sizeof(uint32), bytecode.data());
                compileData.binaries.Emplace(compiledSpirV);
            }

            // Reflect linked program
            auto result = BuildReflection(program, compileData);

            compileData.compiled = result;

            BERSERK_VK_LOG_INFO(BERSERK_TEXT("Compile shader {0} time={1}ms"), compileData.program->GetShaderName(), scopedTimer.GetElapsedMs());
        }

    }
}