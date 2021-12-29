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
#include <rhi/opengl/GLShader.hpp>

BRK_NS_BEGIN

GLShader::GLShader(const RHIShaderDesc &desc) {
    mName = desc.name;
    mLanguage = desc.language;
    mStages = desc.stages;
    mCallback = desc.callback;
    mByteCode = desc.byteCode;
}

GLShader::~GLShader() {
    if (mHandle) {
        GLuint shaders[RHILimits::MAX_SHADER_STAGES];
        GLsizei maxCount = RHILimits::MAX_SHADER_STAGES;
        GLsizei count;

        glGetAttachedShaders(mHandle, maxCount, &count, shaders);
        BRK_GL_CATCH_ERR();

        glDeleteProgram(mHandle);
        BRK_GL_CATCH_ERR();

        for (GLsizei i = 0; i < count; i++) {
            glDeleteShader(shaders[i]);
            BRK_GL_CATCH_ERR();
        }

        mHandle = 0;
    }
}

RHIShader::Status GLShader::GetCompilationStatus() const {
    return mCompilationStatus.load();
}

String GLShader::GetCompilerMessage() const {
    return mCompilationStatus.load() != Status::PendingCompilation ? mCompilerMessage : String();
}

Ref<const RHIShaderMeta> GLShader::GetShaderMeta() const {
    return mCompilationStatus.load() == Status::Compiled ? mMeta.As<const RHIShaderMeta>() : Ref<const RHIShaderMeta>{};
}

void GLShader::Initialize() {
    if (!Engine::Instance().GetRHIDevice().IsSupported(mLanguage)) {
        BRK_ERROR("Shader \"" << mName << "\" language is not supported " << static_cast<int>(mLanguage));
        mCompilationStatus.store(RHIShader::Status::FailedCompile);
        return;
    }
    if (!ValidateStages()) {
        BRK_ERROR("Invalid shader \"" << mName << "\" stages " << static_cast<int>(mLanguage));
        mCompilationStatus.store(RHIShader::Status::FailedCompile);
        return;
    }

    GLsizei current = 0;
    GLuint shaders[RHILimits::MAX_SHADER_STAGES];

    bool withError = false;
    String error;

    for (const auto &module : mStages) {
        assert(!module.sourceCode.empty());

        auto shaderType = GLDefs::GetShaderType(module.type);

        GLuint handle = glCreateShader(shaderType);
        BRK_GL_CATCH_ERR();

        const char *source = module.sourceCode.c_str();
        const char *sources[] = {source};
        GLint length[] = {(GLint) module.sourceCode.length()};

        glShaderSource(handle, 1, sources, length);
        BRK_GL_CATCH_ERR();

        glCompileShader(handle);
        BRK_GL_CATCH_ERR();

        GLint result = 0;
        glGetShaderiv(handle, GL_COMPILE_STATUS, &result);
        BRK_GL_CATCH_ERR();

        if (!result) {
            withError = true;

            int32 logLength;
            glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);
            BRK_GL_CATCH_ERR();

            if (logLength > 0) {
                std::vector<char> log(logLength + 1);

                GLint written;
                glGetShaderInfoLog(handle, logLength, &written, log.data());
                BRK_GL_CATCH_ERR();

                assert(written <= logLength);
                log[written] = '\0';
                error = log.data();
            }

            glDeleteShader(handle);
            BRK_GL_CATCH_ERR();

            break;
        }

        shaders[current] = handle;
        current++;
    }

    if (withError) {
        // Release all created shader stages
        for (GLsizei i = 0; i < current; i++) {
            glDeleteShader(shaders[i]);
            BRK_GL_CATCH_ERR();
        }

        BRK_ERROR("Shader \"" << mName << "\" compilation: Error: " << error);

        mCompilerMessage = std::move(error);
        mCompilationStatus.store(Status::FailedCompile);
        return;
    }

    mHandle = glCreateProgram();
    BRK_GL_CATCH_ERR();

    for (GLsizei i = 0; i < current; i++) {
        glAttachShader(mHandle, shaders[i]);
        BRK_GL_CATCH_ERR();
    }

    glLinkProgram(mHandle);
    BRK_GL_CATCH_ERR();

    GLint status;
    glGetProgramiv(mHandle, GL_LINK_STATUS, &status);

    if (!status) {
        withError = true;

        int32 logLength;
        glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &logLength);
        BRK_GL_CATCH_ERR();

        if (logLength > 0) {
            std::vector<char> log(logLength + 1);

            GLint written;
            glGetShaderInfoLog(mHandle, logLength, &written, log.data());
            BRK_GL_CATCH_ERR();

            assert(written <= logLength);
            log[written] = '\0';
            error = log.data();
        }
    }

    if (withError) {
        // Release all created shader stages
        for (GLsizei i = 0; i < current; i++) {
            glDeleteShader(shaders[i]);
            BRK_GL_CATCH_ERR();
        }

        // Release program handle
        glDeleteProgram(mHandle);
        BRK_GL_CATCH_ERR();

        BRK_ERROR("Shader \"" << mName << "\" compilation: Error: " << error);

        mHandle = 0;
        mCompilerMessage = std::move(error);
        mCompilationStatus.store(Status::FailedCompile);
        return;
    }
    // Create reflection meta information
    InitializeMeta();

    // Remember to notify user, that program is compiled
    mCompilationStatus.store(Status::Compiled);

    // Debug only
    BRK_INFO("Shader \"" << mName << "\" compilation: OK");

    // Notify user
    if (mCallback) {
        auto &scheduler = Engine::Instance().GetScheduler();
        scheduler.ScheduleOnGameThread(std::bind(mCallback, Ref<RHIShader>(this)));
    }
}

bool GLShader::ValidateStages() const {
    // Currently only vs + fs shaders combination is allowed
    assert(mStages.size() == 2);

    bool vs = false;
    bool fs = false;

    for (auto &stage : mStages) {
        if (stage.type == RHIShaderType::Vertex)
            vs = true;
        else if (stage.type == RHIShaderType::Fragment)
            fs = true;
        else
            return false;
    }

    return vs && fs;
}

void GLShader::InitializeMeta() {
    Ref<RHIShaderMeta> meta(new RHIShaderMeta);

    /** Meta name is the same as program has */
    meta->name = GetShaderName();

    auto &inputs = meta->inputs;
    auto &params = meta->params;
    auto &paramBlocks = meta->paramBlocks;
    auto &samplers = meta->samplers;

    // Input attributes

    GLint activeAttributes = 0;
    glGetProgramiv(mHandle, GL_ACTIVE_ATTRIBUTES, &activeAttributes);
    BRK_GL_CATCH_ERR();

    for (uint32 i = 0; i < static_cast<uint32>(activeAttributes); i++) {
        GLchar name[RHILimits::MAX_SHADER_PARAM_NAME];
        GLenum type;
        GLint size;
        GLsizei length;
        GLint location;

        glGetActiveAttrib(mHandle, i, RHILimits::MAX_SHADER_PARAM_NAME, &length, &size, &type, name);
        BRK_GL_CATCH_ERR();

        assert(length < RHILimits::MAX_SHADER_PARAM_NAME);

        location = glGetAttribLocation(mHandle, name);
        BRK_GL_CATCH_ERR();

        RHIShaderMeta::InputAttribute attribute;
        attribute.name = std::move(StringName(name));
        attribute.location = location;
        attribute.type = GLDefs::GetElementType(type);

        if (attribute.type != RHIVertexElementType::Unknown && location != -1) {
            inputs.emplace(attribute.name, attribute);
        }
    }

    // Object Params

    GLint activeUniforms = 0;
    GLuint uniformsIndices[RHILimits::MAX_SHADER_PARAMS_COUNT];
    GLint uniformsOffset[RHILimits::MAX_SHADER_PARAMS_COUNT];
    GLint uniformsBlockIndices[RHILimits::MAX_SHADER_PARAMS_COUNT];
    GLint uniformsArrayStride[RHILimits::MAX_SHADER_PARAMS_COUNT];
    GLint uniformsMatrixStride[RHILimits::MAX_SHADER_PARAMS_COUNT];

    glGetProgramiv(mHandle, GL_ACTIVE_UNIFORMS, &activeUniforms);
    BRK_GL_CATCH_ERR();

    assert(activeUniforms <= RHILimits::MAX_SHADER_PARAMS_COUNT);

    for (uint32 i = 0; i < static_cast<uint32>(activeUniforms); i++) {
        uniformsIndices[i] = i;
    }

    glGetActiveUniformsiv(mHandle, activeUniforms, uniformsIndices, GL_UNIFORM_OFFSET, uniformsOffset);
    BRK_GL_CATCH_ERR();

    glGetActiveUniformsiv(mHandle, activeUniforms, uniformsIndices, GL_UNIFORM_BLOCK_INDEX, uniformsBlockIndices);
    BRK_GL_CATCH_ERR();

    glGetActiveUniformsiv(mHandle, activeUniforms, uniformsIndices, GL_UNIFORM_ARRAY_STRIDE, uniformsArrayStride);
    BRK_GL_CATCH_ERR();

    glGetActiveUniformsiv(mHandle, activeUniforms, uniformsIndices, GL_UNIFORM_MATRIX_STRIDE, uniformsMatrixStride);
    BRK_GL_CATCH_ERR();

    for (uint32 i = 0; i < static_cast<uint32>(activeUniforms); i++) {
        if (uniformsBlockIndices[i] >= 0) {
            // This param within data param block block
            continue;
        }

        GLchar name[RHILimits::MAX_SHADER_PARAM_NAME];
        GLsizei length;
        GLint location;
        GLint array;
        GLenum type;

        glGetActiveUniform(mHandle, i, RHILimits::MAX_SHADER_PARAM_NAME, &length, &array, &type, name);
        BRK_GL_CATCH_ERR();

        assert(length < RHILimits::MAX_SHADER_PARAM_NAME);

        location = glGetUniformLocation(mHandle, name);
        BRK_GL_CATCH_ERR();

        RHIShaderMeta::ObjectParam objectParam;
        objectParam.name = std::move(StringName(String(name, (std::strchr(name, ']') ? length - 3 : length))));
        objectParam.location = location;
        objectParam.arraySize = array;
        objectParam.type = GLDefs::GetShaderParam(type);

        assert(objectParam.type != RHIShaderParamType::Unknown);
        samplers.emplace(objectParam.name, objectParam);
    }

    // Data params within data param blocks (uniform blocks)

    GLint activeUniformBlocks = 0;
    glGetProgramiv(mHandle, GL_ACTIVE_UNIFORM_BLOCKS, &activeUniformBlocks);
    BRK_GL_CATCH_ERR();

    for (uint32 i = 0; i < static_cast<uint32>(activeUniformBlocks); i++) {
        GLchar name[RHILimits::MAX_SHADER_PARAM_NAME];
        GLsizei length;
        GLint size;
        GLint uniformsCount = 0;
        GLint membersIndices[RHILimits::MAX_SHADER_PARAMS_COUNT];

        glGetActiveUniformBlockName(mHandle, i, RHILimits::MAX_SHADER_PARAM_NAME, &length, name);
        BRK_GL_CATCH_ERR();

        glGetActiveUniformBlockiv(mHandle, i, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
        BRK_GL_CATCH_ERR();

        glGetActiveUniformBlockiv(mHandle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniformsCount);
        BRK_GL_CATCH_ERR();

        glGetActiveUniformBlockiv(mHandle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, membersIndices);
        BRK_GL_CATCH_ERR();

        assert(length < RHILimits::MAX_SHADER_PARAM_NAME);
        assert(uniformsCount < RHILimits::MAX_SHADER_PARAMS_COUNT);

        RHIShaderMeta::DataParamBlock paramBlock;
        paramBlock.name = std::move(StringName(name));
        paramBlock.slot = i;// Actual index in the Shader program
        paramBlock.size = size;

        for (uint32 j = 0; j < static_cast<uint32>(uniformsCount); j++) {
            GLenum type;
            GLint array;
            GLuint memberIndex = membersIndices[j];
            GLint matrixStride;

            glGetActiveUniform(mHandle, memberIndex, RHILimits::MAX_SHADER_PARAM_NAME, &length, &array, &type, name);
            BRK_GL_CATCH_ERR();

            glGetActiveUniformsiv(mHandle, 1, &memberIndex, GL_UNIFORM_MATRIX_STRIDE, &matrixStride);
            BRK_GL_CATCH_ERR();

            assert(length < RHILimits::MAX_SHADER_PARAM_NAME);

            RHIShaderMeta::DataParam dataParam;
            dataParam.name = std::move(StringName(String(name, (std::strchr(name, ']') ? length - 3 : length))));
            dataParam.arraySize = array;
            dataParam.matrixStride = GLDefs::IsMatrixType(type) ? static_cast<uint16>(matrixStride) : 0;
            dataParam.arrayStride = uniformsArrayStride[memberIndex] > 0 ? uniformsArrayStride[memberIndex] : 0;
            dataParam.elementSize = GLDefs::GetShaderDataSize(type);
            dataParam.type = GLDefs::GetShaderDataParam(type);
            dataParam.blockSlot = paramBlock.slot;
            dataParam.blockOffset = uniformsOffset[memberIndex];

            params.emplace(dataParam.name, dataParam);
        }

        paramBlocks.emplace(paramBlock.name, paramBlock);
    }

    mMeta = std::move(meta);
}

void GLShader::BindUniformBlock(uint32 binding) const {
    glUniformBlockBinding(mHandle, binding, binding);
    BRK_GL_CATCH_ERR();
}

void GLShader::Use() const {
    glUseProgram(mHandle);
    BRK_GL_CATCH_ERR();
}

BRK_NS_END