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

#ifndef BERSERK_SHADERARCHETYPE_HPP
#define BERSERK_SHADERARCHETYPE_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <render/shader/Shader.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup render
 * @{
 */

/**
 * @class ShaderArchetype
 * @brief Handles source code pre-processing of shaders with this archetype
 */
class ShaderArchetype {
public:
    /** @brief Processing input data */
    struct InputData {
        ShaderCompileOptions defines{};
        String sourceCode{};
        uint32 passIndex{};
        RHIType rhiType{};
        RHIShaderType stage{};
        RHIShaderLanguage language{};
    };

    /** @brief Processing output data */
    struct OutputData {
        bool failed = false;
        String error{};
        Ref<Data> sourceCode{};
        Ref<RHIVertexDeclaration> declaration{};
    };

    BRK_API virtual ~ShaderArchetype() = default;
    BRK_API virtual const StringName &GetArchetype() const = 0;
    BRK_API virtual const ShaderCompileOptions &GetAvailableOptions() const = 0;
    BRK_API virtual void DefineVariation(const ShaderCompileOptions &defines, ShaderVariation &variation) = 0;
    BRK_API virtual void Process(const InputData &inputData, OutputData &outputData) = 0;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_SHADERARCHETYPE_HPP
