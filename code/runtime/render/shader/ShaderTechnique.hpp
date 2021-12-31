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

#ifndef BERSERK_SHADERTECHNIQUE_HPP
#define BERSERK_SHADERTECHNIQUE_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/string/StringName.hpp>
#include <core/templates/Mask.hpp>
#include <render/shader/ShaderPass.hpp>

#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup render
 * @{
 */

/**
 * @class ShaderTechniqueTag
 * @brief Technique tags used to select techniques
 */
enum class ShaderTechniqueTag {

};

/**
 * @class ShaderTechniqueTags
 * @brief Set of tags
 */
using ShaderTechniqueTags = Mask<ShaderTechniqueTag>;

/**
 * @class ShaderTechnique
 * @brief Represents particular approach to draw single object
 */
class ShaderTechnique final : public RefCnt {
public:
    /** CreateFromImage empty technique */
    BRK_API ShaderTechnique() = default;
    BRK_API ~ShaderTechnique() override = default;

    /** Set technique name */
    BRK_API void SetName(StringName name);
    /** Set tags */
    BRK_API void SetTags(const ShaderTechniqueTags &tags);
    /** Set type of api for the technique */
    BRK_API void SetRHIType(RHIType rhiType);
    /** Set type of shader language used to write technique */
    BRK_API void SetRHILanguage(RHIShaderLanguage language);
    /** Set list of passes */
    BRK_API void SetPasses(std::vector<Ref<ShaderPass>> passes);
    /** Set shader this technique belongs to */
    BRK_API void SetShader(class Shader *shader);

    /** Fill pipeline descriptor for specified pass */
    BRK_API void FillPipelineDesc(RHIGraphicsPipelineDesc &desc, uint32 passIdx) const;

    /** @return True if technique compiled */
    BRK_API bool IsCompiled() const;

    /** @return Get pass by name */
    BRK_API Ref<const ShaderPass> GetPass(const StringName &name) const;
    /** @return Get pass by index */
    BRK_API Ref<const ShaderPass> GetPass(uint32 passIdx) const;
    /** @return Number of passes */
    BRK_API size_t GetPassesCount() const { return mPasses.size(); }

    /** @return Technique name */
    BRK_API const StringName &GetName() const { return mName; }
    /** @return Set of tags */
    BRK_API const ShaderTechniqueTags &GetTags() const { return mTags; }
    /** @return Type of api for the technique */
    BRK_API RHIType GetRHIType() const { return mRHIType; }
    /** @return Type of shader language used to write technique */
    BRK_API RHIShaderLanguage GetRHILanguage() const { return mRHILanguage; }
    /** @return List of passes */
    BRK_API const std::vector<Ref<ShaderPass>> &GetPasses() const { return mPasses; }
    /** @return Shader this technique belongs to */
    BRK_API class Shader *GetShader() const { return mShader; }

private:
    /** Technique name */
    StringName mName;
    /** Set of tags */
    ShaderTechniqueTags mTags;
    /** Type of api for the technique */
    RHIType mRHIType = RHIType::Unknown;
    /** Language this technique is written in */
    RHIShaderLanguage mRHILanguage = RHIShaderLanguage::Unknown;
    /** List of passes */
    std::vector<Ref<ShaderPass>> mPasses;
    /** Shader this technique belongs to */
    class Shader *mShader = nullptr;
    /** Cached compile status */
    mutable bool mIsCompiled = false;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_SHADERTECHNIQUE_HPP
