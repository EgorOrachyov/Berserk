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

#ifndef BERSERK_SHADERCOMPILER_HPP
#define BERSERK_SHADERCOMPILER_HPP

#include <render/shader/Shader.hpp>

#include <tinyxml2.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup render
 * @{
 */

/**
 * @class ShaderCompiler
 * @brief Responsible to compile shaders in engine .shader.xml format
 */
class ShaderCompiler final {
public:
    /** @brief Compilation result */
    struct CompilationResult {
        String error;
        uint32 line{};
        Ref<Shader> shader;
    };

    BRK_API ShaderCompiler() = default;
    BRK_API ~ShaderCompiler() = default;

    /**
     * @brief Compile shader with specified full path and options
     *
     * @note Accepts shaders in `.shader.xml` format
     *
     * @param filepath Full file path to the shader file on disc
     * @param options Options to compile with
     *
     * @return Compilation result
     */
    BRK_API CompilationResult Compile(const String &filepath, const Ref<ShaderCompileOptions> &options);

private:
    bool ParseShader(tinyxml2::XMLElement *element);
    bool ParseOptions(tinyxml2::XMLElement *element);
    bool ParseParams(tinyxml2::XMLElement *element);
    bool ParseTechniques(tinyxml2::XMLElement *element);
    bool ParseTechnique(tinyxml2::XMLElement *element);
    bool ParsePass(tinyxml2::XMLElement *element);

    static bool ParseParamType(const char *typeName, ShaderParamType &type, RHIShaderDataType &dataType, RHIShaderParamType &paramType);

private:
    CompilationResult mResult;

    Ref<Shader> mShader;
    Ref<ShaderCompileOptions> mOptions;
    Ref<ShaderParams> mParams;
    Ref<ShaderTechnique> mTechnique;
    Ref<ShaderPass> mPass;
    std::vector<ShaderOption> mAllOptions;
    std::shared_ptr<ShaderArchetype> mArchetype{};
    uint32 mPassIndex{};
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_SHADERCOMPILER_HPP
