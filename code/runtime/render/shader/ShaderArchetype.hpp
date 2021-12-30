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
#include <render/mesh/MeshFormats.hpp>
#include <render/shader/Shader.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup render
 * @{
 */

/**
 * @class ShaderVariationHelper
 * @brief Assist in building variation key from options
 */
class ShaderVariationHelper {
public:
    /** Register new option */
    BRK_API void AddOption(const StringName &option) {
        auto find = mOptions.find(option);
        if (find == mOptions.end()) mOptions[option] = mNextOption++;
    }

    /** @return Get option id mapping */
    BRK_API uint32 GetOption(const StringName &option) {
        auto find = mOptions.find(option);
        if (find != mOptions.end()) return find->second;
        return mOptions[option] = mNextOption++;
    }

    /** @return True if option registered */
    BRK_API bool IsRegistered(const StringName &option) {
        auto find = mOptions.find(option);
        return find != mOptions.end();
    }

private:
    std::unordered_map<StringName, uint32> mOptions;
    uint32 mNextOption = 0;
};

/**
 * @class ShaderArchetype
 * @brief Handles source code pre-processing of shaders with this archetype
 */
class ShaderArchetype {
public:
    /** Name of params block generated from material params */
    static constexpr const char *const SHADER_PARAMS_BLOCK = BRK_TEXT("ShaderParams");

    /** @brief Processing input data */
    struct InputData {
        Ref<ShaderCompileOptions> options{};
        Ref<ShaderParams> params{};
        String vertexCode{};
        String fragmentCode{};
        uint32 passIndex{};
        RHIShaderLanguage language{};
    };

    /** @brief Processing output data */
    struct OutputData {
        bool failed = false;
        String error{};
        RHIShaderLanguage language{};
        String vertexCode;
        String fragmentCode;
    };

    BRK_API virtual ~ShaderArchetype() = default;
    BRK_API virtual const StringName &GetArchetype() const = 0;
    BRK_API virtual const std::vector<RHIShaderLanguage> &GetSupportedLanguages() const = 0;
    BRK_API virtual bool IsSupportedLanguage(RHIShaderLanguage language) const;
    BRK_API virtual void DefineOptions(std::vector<ShaderOption> &options) const = 0;
    BRK_API virtual void DefineVariation(const ShaderCompileOptions &options, ShaderVariation &variation) = 0;
    BRK_API virtual void DefineFormat(const ShaderCompileOptions &options, MeshFormat &format) = 0;
    BRK_API virtual void Process(const InputData &inputData, OutputData &outputData) = 0;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_SHADERARCHETYPE_HPP
