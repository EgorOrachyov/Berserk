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
#include <render/archetypes/ShaderArchetypeBase.hpp>
#include <render/archetypes/UtilsGLSL.hpp>

#include <sstream>
#include <vector>

BRK_NS_BEGIN

const StringName &ShaderArchetypeBase::GetArchetype() const {
    return mArchetype;
}
const std::vector<RHIShaderLanguage> &ShaderArchetypeBase::GetSupportedLanguages() const {
    return mLanguages;
}

void ShaderArchetypeBase::DefineOptions(std::vector<ShaderOption> &options) const {
    options.emplace_back(ShaderOption{StringName("D_NO_UV"), String("No uv attribute in the mesh")});
}

void ShaderArchetypeBase::DefineVariation(const ShaderCompileOptions &options, ShaderVariation &variation) {
    variation = 0u;

    for (const auto &option : options.Get())
        variation = variation | (static_cast<uint32>(1u) << static_cast<uint32>(mVariationHelper.GetOption(option)));
}

void ShaderArchetypeBase::DefineFormat(const ShaderCompileOptions &options, MeshFormat &format) {
    format.Set(MeshAttribute::Position);
    format.Set(MeshAttribute::Normal);
    format.Set(MeshAttribute::Color);

    if (!options.IsSet(StringName("D_NO_UV")))
        format.Set(MeshAttribute::UV);
}

void ShaderArchetypeBase::Process(const ShaderArchetype::InputData &inputData, ShaderArchetype::OutputData &outputData) {
    std::stringstream vs;

    bool hasUV = !inputData.options->IsSet(StringName("D_NO_UV"));

    vs << "#version 410 core\n"
       << "layout (location = 0) in vec3 _vPos;\n"
       << "layout (location = 1) in vec3 _vNorm;\n"
       << "layout (location = 2) in vec3 _vColor;\n";
    if (hasUV)
        vs << "layout (location = 3) in vec2 _vUV;\n";

    UtilsGLSL::GenerateDefines(*inputData.options, vs);

    vs << "out vec3 _fsWorldPos;\n"
       << "out vec3 _fsWorldNorm;\n"
       << "out vec3 _fsColor;\n";
    if (hasUV)
        vs << "out vec2 _fsUV;\n";

    vs << "struct Params {\n"
       << "  vec4 projPos;\n"
       << "  vec3 worldPos;\n"
       << "  vec3 worldNorm;\n"
       << "  vec3 color;\n"
       << "  vec2 uv;\n"
       << "};\n";

    vs << "vec3 getPos() { return _vPos; }\n"
       << "vec3 getNorm() { return _vNorm; }\n"
       << "vec3 getColor() { return _vColor; }\n";
    if (hasUV)
        vs << "vec2 getUV() { return _vUV; }\n";

    UtilsGLSL::GenerateStruct(SHADER_PARAMS_BLOCK, "std140", *inputData.params, vs);
    UtilsGLSL::GenerateUserCode(inputData.vertexCode, vs);

    vs << "void main() { \n"
       << "  Params params;\n"
       << "  params.projPos = vec4(_vPos, 1.0f);\n"
       << "  params.worldPos = _vPos;\n"
       << "  params.worldNorm = _vNorm;\n"
       << "  params.color = _vColor;\n";

    if (hasUV)
        vs << "  params.uv = _vUV;\n";

    vs << "  vertex(params);\n"
       << "  gl_Position = params.projPos;\n"
       << "  _fsWorldPos = params.worldPos;\n"
       << "  _fsWorldNorm = params.worldNorm;\n"
       << "  _fsColor = params.color;\n";
    if (hasUV)
        vs << "  _fsUV = params.uv;\n";
    vs << "}\n";

    std::stringstream fs;

    fs << "#version 410 core\n"
       << "layout (location = 0) out vec4 _fColor;\n";

    UtilsGLSL::GenerateDefines(*inputData.options, fs);

    fs << "in vec3 _fsWorldPos;\n"
       << "in vec3 _fsWorldNorm;\n"
       << "in vec3 _fsColor;\n";
    if (hasUV)
        fs << "in vec2 _fsUV;\n";

    fs << "struct Params {\n"
       << "  vec4 outColor;\n"
       << "};\n";

    fs << "vec3 getWorldPos() { return _fsWorldPos; }\n"
       << "vec3 getWorldNorm() { return _fsWorldNorm; }\n"
       << "vec3 getColor() { return _fsColor; }\n";
    if (hasUV)
        fs << "vec2 getUV() { return _fsUV; }\n";

    UtilsGLSL::GenerateStruct(SHADER_PARAMS_BLOCK, "std140", *inputData.params, fs);
    UtilsGLSL::GenerateUniformParams(*inputData.params, fs);
    UtilsGLSL::GenerateUserCode(inputData.fragmentCode, fs);

    fs << "void main() { \n"
       << "  Params params;\n"
       << "  params.outColor = vec4(_fsColor, 1.0f);\n"
       << "  fragment(params);\n"
       << "  _fColor = params.outColor;\n"
       << "}\n";

    outputData.vertexCode = vs.str();
    outputData.fragmentCode = fs.str();
    outputData.language = RHIShaderLanguage::GLSL410GL;

#if 0
    auto &output = Engine::Instance().GetOutput();
    output.Write("Vertex shader code\n");
    output.Write(outputData.vertexCode);
    output.Write("\nFragment shader code\n");
    output.Write(outputData.fragmentCode);
    output.Write("\n");
#endif
}

BRK_NS_END
