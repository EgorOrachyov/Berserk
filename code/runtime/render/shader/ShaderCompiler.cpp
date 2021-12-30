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
#include <render/shader/ShaderCompiler.hpp>

#include <tinyxml2.hpp>

BRK_NS_BEGIN

ShaderCompiler::CompilationResult ShaderCompiler::Compile(const String &filepath, const Ref<ShaderCompileOptions> &options) {
    auto &engine = Engine::Instance();
    auto &fileSystem = engine.GetFileSystem();

    auto file = fileSystem.OpenFile(filepath, "rb");
    if (!file) {
        mResult.error = BRK_TEXT("Failed to load shader file content from disc");
        return mResult;
    }

    tinyxml2::XMLDocument document;
    tinyxml2::XMLError error = document.LoadFile(file);

    fileSystem.CloseFile(file);

    if (error != tinyxml2::XML_SUCCESS) {
        mResult.error = BRK_TEXT("Failed to parse shader file");
        return mResult;
    }

    mOptions = options;

    if (!ParseShader(document.FirstChildElement("shader"))) {
        mResult.shader.Reset();
        return mResult;
    }

    mResult.shader = mShader;
    mResult.shader->SetLoadPath(filepath);
    mResult.shader->SetOptions(mOptions);
    return mResult;
}

#define BRK_SC_ERROR(what)          \
    mResult.error = BRK_TEXT(what); \
    return false;

#define BRK_SC_ERROR_W(what, where)       \
    mResult.line = (where)->GetLineNum(); \
    mResult.error = BRK_TEXT(what);       \
    return false;

#define BRK_SC_CHECK_ERROR(condition, what) \
    if (!(condition)) {                     \
        BRK_SC_ERROR(what);                 \
    } else {                                \
    }

#define BRK_SC_CHECK_ERROR_W(condition, what, where) \
    if (!(condition)) {                              \
        BRK_SC_ERROR_W(what, where);                 \
    } else {                                         \
    }

bool ShaderCompiler::ParseShader(tinyxml2::XMLElement *element) {
    if (!element) {
        BRK_SC_ERROR("No shader no in the document");
    }

    auto name = element->FindAttribute("name");
    auto archetype = element->FindAttribute("archetype");
    auto description = element->FindAttribute("description");

    assert(name);
    assert(archetype);

    BRK_SC_CHECK_ERROR_W(name, "Shader name must be provided", element);
    BRK_SC_CHECK_ERROR_W(archetype, "Shader archetype must be provided", element);

    StringName archetypeName(archetype->Value());
    auto &shaderManager = Engine::Instance().GetRenderEngine().GetShaderManager();
    mArchetype = shaderManager.FindArchetype(archetypeName);

    BRK_SC_CHECK_ERROR_W(mArchetype, "No archetype registered for this shader", element);

    mShader = Ref<Shader>(new Shader);
    mShader->SetName(StringName(name->Value()));
    mShader->SetArchetype(std::move(archetypeName));

    ShaderVariation variation;
    MeshFormat format;
    mArchetype->DefineVariation(*mOptions, variation);
    mArchetype->DefineFormat(*mOptions, format);

    mShader->SetVariation(variation);
    mShader->SetFormat(format);

    if (description)
        mShader->SetDescription(description->Value());

    if (!ParseOptions(element->FirstChildElement("options"))) {
        return false;
    }

    mShader->SetAllOptions(std::move(mAllOptions));

    if (!ParseParams(element->FirstChildElement("params"))) {
        return false;
    }

    mShader->SetParams(mParams);

    if (!ParseTechniques(element->FirstChildElement("techniques"))) {
        return false;
    }

    return true;
}

bool ShaderCompiler::ParseOptions(tinyxml2::XMLElement *element) {
    // Get default archetype exposed options
    mArchetype->DefineOptions(mAllOptions);

    if (!element) {
        // Ok, shader has no options
        return true;
    }

    auto option = element->FirstChildElement("option");
    while (option) {
        auto name = option->FindAttribute("name");
        auto description = option->FindAttribute("description");

        assert(name);
        BRK_SC_CHECK_ERROR_W(name, "Option name must be provided", option);

        mAllOptions.emplace_back();
        auto &shaderOption = mAllOptions.back();
        shaderOption.name = std::move(StringName(name->Value()));
        shaderOption.description = (description ? description->Value() : "");

        option = option->NextSiblingElement("option");
    }

    return true;
}

bool ShaderCompiler::ParseParams(tinyxml2::XMLElement *element) {
    if (!element) {
        // Ok, shader has no params
        mParams = Ref<ShaderParams>(new ShaderParams({}));
        return true;
    }

    std::vector<ShaderParam> paramsList;
    auto paramNode = element->FirstChildElement("param");

    while (paramNode) {
        auto name = paramNode->FindAttribute("name");
        auto uiName = paramNode->FindAttribute("uiName");
        auto type = paramNode->FindAttribute("type");
        auto arraySize = paramNode->FindAttribute("arraySize");
        auto defaultValue = paramNode->FindAttribute("default");
        auto description = paramNode->FindAttribute("description");

        assert(name);
        assert(type);

        BRK_SC_CHECK_ERROR_W(name, "Shader param name must be provided", paramNode);
        BRK_SC_CHECK_ERROR_W(type, "Shader param type must be provided", paramNode);

        // Add new param and get it ref
        paramsList.emplace_back();
        auto &param = paramsList.back();

        // Parse type first
        if (!ParseParamType(type->Value(), param.type, param.typeData, param.typeParam)) {
            BRK_SC_ERROR_W("Failed to parse param type", type);
        }

        param.name = std::move(StringName(name->Value()));

        // Add optional values
        if (uiName)
            param.uiName = std::move(StringName(uiName->Value()));
        if (description)
            param.description = description->Value();
        if (defaultValue)
            param.defaultValue = defaultValue->Value();
        if (arraySize) {
            param.arraySize = arraySize->UnsignedValue();
            BRK_SC_CHECK_ERROR_W(param.arraySize > 0, "Array size must be greater then zero", paramNode);
        }

        paramNode = paramNode->NextSiblingElement("param");
    }

    // Compile shader params layout
    mParams = Ref<ShaderParams>(new ShaderParams(std::move(paramsList)));

    return true;
}

bool ShaderCompiler::ParseTechniques(tinyxml2::XMLElement *element) {
    BRK_SC_CHECK_ERROR(element, "No techniques section in the shader");
    auto technique = element->FirstChildElement();

    BRK_SC_CHECK_ERROR_W(technique, "No technique to parse in techniques section", element);

    std::vector<Ref<ShaderTechnique>> shaderTechniques;

    while (technique) {
        if (!ParseTechnique(technique))
            return false;

        shaderTechniques.push_back(mTechnique);
        technique = technique->NextSiblingElement("technique");
    }

    mShader->SetTechniques(std::move(shaderTechniques));

    return true;
}

bool ShaderCompiler::ParseTechnique(tinyxml2::XMLElement *element) {
    assert(element);

    auto name = element->FindAttribute("name");
    auto language = element->FindAttribute("language");
    auto passes = element->FirstChildElement("passes");

    BRK_SC_CHECK_ERROR_W(name, "Shader technique name must be provided", element);
    BRK_SC_CHECK_ERROR_W(language, "Shader technique language must be provided", element);
    BRK_SC_CHECK_ERROR_W(passes, "Shader technique must have passes section", element);

    auto pass = passes->FirstChildElement("pass");
    auto rhiLanguage = RHIParseShaderLanguage(language->Value());

    BRK_SC_CHECK_ERROR_W(pass, "No passes in shader technique passes section", element);
    BRK_SC_CHECK_ERROR_W(rhiLanguage != RHIShaderLanguage::Unknown, "Invalid language specified", element);
    BRK_SC_CHECK_ERROR_W(mArchetype->IsSupportedLanguage(rhiLanguage), "Language is not supported by archetype", element);

    mTechnique = Ref<ShaderTechnique>(new ShaderTechnique);
    mTechnique->SetName(StringName(name->Value()));
    mTechnique->SetRHILanguage(rhiLanguage);
    mTechnique->SetShader(mShader.Get());

    std::vector<Ref<ShaderPass>> shaderPasses;

    mPassIndex = 0;

    while (pass) {
        if (!ParsePass(pass))
            return false;

        shaderPasses.push_back(mPass);
        pass = pass->NextSiblingElement("pass");
        mPassIndex += 1;
    }

    mTechnique->SetPasses(std::move(shaderPasses));

    return true;
}

bool ShaderCompiler::ParsePass(tinyxml2::XMLElement *element) {
    assert(element);

    auto name = element->FindAttribute("name");
    auto vertex = element->FirstChildElement("vertex");
    auto fragment = element->FirstChildElement("fragment");

    assert(name);
    assert(vertex);
    assert(fragment);

    BRK_SC_CHECK_ERROR_W(name, "Pass name must be provided", element);
    BRK_SC_CHECK_ERROR_W(vertex, "Vertex shader must be provided", element);
    BRK_SC_CHECK_ERROR_W(fragment, "Fragment shader must be provided", element);

    assert(mArchetype);

    mPass = Ref<ShaderPass>(new ShaderPass);
    mPass->SetName(StringName(name->Value()));
    mPass->SetTechnique(mTechnique.Get());

    ShaderArchetype::InputData inputData;
    inputData.language = mTechnique->GetRHILanguage();
    inputData.options = mOptions;
    inputData.params = mParams;
    inputData.vertexCode = vertex->GetText();
    inputData.fragmentCode = fragment->GetText();
    inputData.passIndex = mPassIndex;

    ShaderArchetype::OutputData outputData;
    mArchetype->Process(inputData, outputData);

    if (outputData.failed) {
        mResult.line = element->GetLineNum();
        mResult.error = outputData.error;
        return false;
    }

    RHIShaderDesc shaderDesc;
    shaderDesc.language = outputData.language;
    shaderDesc.name = mPass->GetName();
    shaderDesc.stages.resize(2);
    shaderDesc.stages[0].type = RHIShaderType::Vertex;
    shaderDesc.stages[0].sourceCode = std::move(outputData.vertexCode);
    shaderDesc.stages[1].type = RHIShaderType::Fragment;
    shaderDesc.stages[1].sourceCode = std::move(outputData.fragmentCode);
    mPass->SetShader(Engine::Instance().GetRHIDevice().CreateShader(shaderDesc));

    return true;
}

bool ShaderCompiler::ParseParamType(const char *typeName, ShaderParamType &type, RHIShaderDataType &dataType, RHIShaderParamType &paramType) {
    static String s_float1("float1");
    static String s_float2("float2");
    static String s_float3("float3");
    static String s_float4("float4");
    static String s_mat2("mat2");
    static String s_mat3("mat3");
    static String s_mat4("mat4");
    static String s_texture2d("texture2d");
    static String s_texture2dArray("texture2dArray");
    static String s_textureCube("textureCube");
    static String s_texture3d("texture3d");

    if (s_float1 == typeName) {
        type = ShaderParamType::Data;
        dataType = RHIShaderDataType::Float1;
    } else if (s_float2 == typeName) {
        type = ShaderParamType::Data;
        dataType = RHIShaderDataType::Float2;
    } else if (s_float3 == typeName) {
        type = ShaderParamType::Data;
        dataType = RHIShaderDataType::Float3;
    } else if (s_float4 == typeName) {
        type = ShaderParamType::Data;
        dataType = RHIShaderDataType::Float4;
    } else if (s_mat2 == typeName) {
        type = ShaderParamType::Data;
        dataType = RHIShaderDataType::Mat2;
    } else if (s_mat3 == typeName) {
        type = ShaderParamType::Data;
        dataType = RHIShaderDataType::Mat3;
    } else if (s_mat4 == typeName) {
        type = ShaderParamType::Data;
        dataType = RHIShaderDataType::Mat4;
    } else if (s_texture2d == typeName) {
        type = ShaderParamType::Texture;
        paramType = RHIShaderParamType::Sampler2d;
    } else if (s_texture2dArray == typeName) {
        type = ShaderParamType::Texture;
        paramType = RHIShaderParamType::Sampler2dArray;
    } else if (s_textureCube == typeName) {
        type = ShaderParamType::Texture;
        paramType = RHIShaderParamType::SamplerCube;
    } else if (s_texture3d == typeName) {
        type = ShaderParamType::Texture;
        paramType = RHIShaderParamType::Sampler3d;
    } else
        return false;

    return true;
}

#undef BRK_SC_ERROR_W
#undef BRK_SC_ERROR
#undef BRK_SC_CHECK_ERROR_W
#undef BRK_SC_CHECK_ERROR

BRK_NS_END
