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

#ifndef BERSERK_SHADER_HPP
#define BERSERK_SHADER_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <render/mesh/MeshFormats.hpp>
#include <render/shader/ShaderParams.hpp>
#include <render/shader/ShaderPass.hpp>
#include <render/shader/ShaderTechnique.hpp>

#include <unordered_set>
#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup render
 * @{
 */

/**
 * @class ShaderVariation
 * @brief Describes variation of compiled shader of the single type (max 64 flags)
 */
using ShaderVariation = uint32;

/**
 * @class ShaderOption
 * @brief Single option exposed by the shader
 */
struct ShaderOption {
    StringName name;
    String description;
};

/**
 * @class ShaderCompileOptions
 * @brief Options passed to compile shader
 */
class ShaderCompileOptions final : public RefCnt {
public:
    BRK_API ShaderCompileOptions() = default;
    BRK_API ~ShaderCompileOptions() override = default;

    BRK_API void Set(const StringName &option) { mValues.emplace(option); }
    BRK_API void Clear() { mValues.clear(); }
    BRK_API size_t GetCount() const { return mValues.size(); }
    BRK_API const std::unordered_set<StringName> &Get() const { return mValues; }

private:
    std::unordered_set<StringName> mValues;
};

/**
 * @class Shader
 * @brief Defines complete rendering flow for the object
 */
class Shader final : public RefCnt {
public:
    BRK_API Shader() = default;
    BRK_API ~Shader() override = default;

    BRK_API void SetName(StringName name);
    BRK_API void SetArchetype(StringName archetype);
    BRK_API void SetLoadPath(String loadPath);
    BRK_API void SetDescription(String description);
    BRK_API void SetVariation(ShaderVariation variation);
    BRK_API void SetFormat(MeshFormat format);
    BRK_API void SetOptions(Ref<ShaderCompileOptions> options);
    BRK_API void SetTechniques(std::vector<Ref<ShaderTechnique>> techniques);
    BRK_API void SetAllOptions(std::vector<ShaderOption> options);
    BRK_API void SetParams(Ref<ShaderParams> params);

    BRK_API Ref<const ShaderTechnique> GetTechnique(const StringName &name) const;
    BRK_API Ref<const ShaderTechnique> GetTechnique(uint32 id) const;
    BRK_API Ref<const ShaderTechnique> FindFirstTechnique(const ShaderTechniqueTags &tags, bool mustBeCompiled = true) const;
    BRK_API size_t GetTechniquesCount() const { return mTechniques.size(); }

    BRK_API const StringName &GetName() const { return mName; }
    BRK_API const StringName &GetArchetype() const { return mArchetype; }
    BRK_API const String &GetLoadPath() const { return mLoadPath; }
    BRK_API const String &GetDescription() const { return mDescription; }
    BRK_API const ShaderVariation &GetVariation() const { return mVariation; }
    BRK_API const MeshFormat &GetFormat() const { return mFormat; }
    BRK_API const std::vector<Ref<ShaderTechnique>> &GetTechniques() const { return mTechniques; }
    BRK_API const std::vector<ShaderOption> &GetAllOptions() const { return mAllOptions; }
    BRK_API const Ref<ShaderParams> &GetParams() const { return mParams; }
    BRK_API const Ref<ShaderCompileOptions> &GetOptions() const { return mOptions; }

private:
    StringName mName;               /** Shader name */
    StringName mArchetype;          /** Shader archetype */
    String mLoadPath;               /** Cached loading path of the file */
    String mDescription;            /** Optional description */
    ShaderVariation mVariation = 0; /** Variation, used to compile this shader */
    MeshFormat mFormat;             /** Input configuration (common for all techniques/passes) */

    std::vector<Ref<ShaderTechnique>> mTechniques; /** List of shader techniques */
    std::vector<ShaderOption> mAllOptions;         /** List of all available (potentially) options */

    Ref<ShaderParams> mParams;          /** Shader params with packing info */
    Ref<ShaderCompileOptions> mOptions; /** List of shader options passed through compilation */
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_SHADER_HPP
