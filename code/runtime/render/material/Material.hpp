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

#ifndef BERSERK_MATERIAL_HPP
#define BERSERK_MATERIAL_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/math/TMatMxN.hpp>
#include <core/math/TVecN.hpp>
#include <render/material/MaterialParams.hpp>
#include <render/shader/Shader.hpp>
#include <rhi/RHISampler.hpp>
#include <rhi/RHITexture.hpp>

#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup render
 * @{
 */

/**
 * @class Material
 * @brief Controls how object is actually rendered
 *
 * Material is composed of the shader object and a set of material params.
 * Shader object defines the set of available params for rendering settings
 * and provides a set of techniques, which define a gpu passes used to actually
 * draw an object. Material params provide user the ability to easily set
 * params to tweak rendering of the concrete object.
 *
 * @see Shader
 * @see ShaderParams
 * @see MaterialParams
 */
class Material final : public RefCnt {
public:
    /**
     * @brief Creates new material instance from provided shader
     *
     * Provided shader must be valid, loaded and compiled shader object
     * with fully set state. Material will select the best available
     * technique from this shader and set its as technique for rendering.
     *
     * @param shader Valid shader object
     */
    BRK_API explicit Material(Ref<const Shader> shader);

    BRK_API ~Material() override = default;

    /** Set material name */
    BRK_API void SetName(StringName name);
    /** Set optional material description  */
    BRK_API void SetDescription(String description);

    /**
     * @brief Set material texture parameter
     *
     * Updates material parameter value.
     * Makes material in dirty state, so requires actual rhi parameters repacking.
     *
     * @param name Parameter name
     * @param texture Texture object to set
     * @param sampler Sampler object to set
     * @param arrayIndex Optional array index to modify; for not array params by default is 0
     */
    BRK_API void SetTexture(const StringName &name, Ref<RHITexture> texture, Ref<RHISampler> sampler, uint32 arrayIndex = 0);

    /**
     * @brief Set material float parameter
     *
     * Updates material parameter value.
     * Makes material in dirty state, so requires actual rhi parameters repacking.
     *
     * @param name Parameter name
     * @param value Parameter value
     * @param arrayIndex Optional array index to modify; for not array params by default is 0
     */
    BRK_API void SetFloat1(const StringName &name, float value, uint32 arrayIndex = 0);

    /**
     * @brief Set material float vec2 parameter
     *
     * Updates material parameter value.
     * Makes material in dirty state, so requires actual rhi parameters repacking.
     *
     * @param name Parameter name
     * @param value Parameter value
     * @param arrayIndex Optional array index to modify; for not array params by default is 0
     */
    BRK_API void SetFloat2(const StringName &name, const Vec2f &value, uint32 arrayIndex = 0);

    /**
     * @brief Set material float vec3 parameter
     *
     * Updates material parameter value.
     * Makes material in dirty state, so requires actual rhi parameters repacking.
     *
     * @param name Parameter name
     * @param value Parameter value
     * @param arrayIndex Optional array index to modify; for not array params by default is 0
     */
    BRK_API void SetFloat3(const StringName &name, const Vec3f &value, uint32 arrayIndex = 0);

    /**
     * @brief Set material float vec4 parameter
     *
     * Updates material parameter value.
     * Makes material in dirty state, so requires actual rhi parameters repacking.
     *
     * @param name Parameter name
     * @param value Parameter value
     * @param arrayIndex Optional array index to modify; for not array params by default is 0
     */
    BRK_API void SetFloat4(const StringName &name, const Vec4f &value, uint32 arrayIndex = 0);

    /**
     * @brief Set material float mat2x2 parameter
     *
     * Updates material parameter value.
     * Makes material in dirty state, so requires actual rhi parameters repacking.
     *
     * @param name Parameter name
     * @param value Parameter value
     * @param arrayIndex Optional array index to modify; for not array params by default is 0
     */
    BRK_API void SetMat2(const StringName &name, const Mat2x2f &value, uint32 arrayIndex = 0);

    /**
     * @brief Set material float mat3x3 parameter
     *
     * Updates material parameter value.
     * Makes material in dirty state, so requires actual rhi parameters repacking.
     *
     * @param name Parameter name
     * @param value Parameter value
     * @param arrayIndex Optional array index to modify; for not array params by default is 0
     */
    BRK_API void SetMat3(const StringName &name, const Mat3x3f &value, uint32 arrayIndex = 0);

    /**
     * @brief Set material float mat4x4 parameter
     *
     * Updates material parameter value.
     * Makes material in dirty state, so requires actual rhi parameters repacking.
     *
     * @param name Parameter name
     * @param value Parameter value
     * @param arrayIndex Optional array index to modify; for not array params by default is 0
     */
    BRK_API void SetMat4(const StringName &name, const Mat4x4f &value, uint32 arrayIndex = 0);

    /**
     * @brief Update rhi-set of the material
     *
     * Must be called by the material owner before rendering
     * to update actual state of material params and data on GPU-side.
     */
    BRK_API void UpdatePack();

    /** @return True if material is dirty and requires repacking */
    BRK_API bool IsDirty() const { return mIsDirty; }

    BRK_API const StringName &GetName() const { return mName; }
    BRK_API const String &GetDescription() const { return mDescription; }
    BRK_API const Ref<const Shader> &GetShader() const { return mShader; }
    BRK_API const Ref<const ShaderTechnique> &GetTechnique() const { return mTechnique; }
    BRK_API const ShaderVariation &GetVariation() const { return mShader->GetVariation(); }

    /** @return Packed material params for rendering on GPU */
    BRK_API Ref<const MaterialParams> GetPackedParams() const { return mPackedParams.As<const MaterialParams>(); }

private:
    friend class MaterialParams;

    void SetDataParam(const StringName &name, RHIShaderDataType type, const void *data, uint32 arrayIndex);
    const std::vector<Ref<RHISampler>> &GetSamplers() const { return mSamplers; }
    const std::vector<Ref<RHITexture>> &GetTextures() const { return mTextures; }
    const std::vector<unsigned char> &GetDataParams() const { return mDataParams; }

private:
    StringName mName;    /** Name of the material */
    String mDescription; /** Optional description text */

    Ref<const Shader> mShader;             /** Base shader of this material */
    Ref<const ShaderTechnique> mTechnique; /** Selected active technique for rendering */
    Ref<MaterialParams> mPackedParams;     /** Packed material params ready for gpu usage */

    std::vector<Ref<RHISampler>> mSamplers; /** Bound samplers (stored separately, bound to concrete texture unit) */
    std::vector<Ref<RHITexture>> mTextures; /** Bound textures (each texture for each array unit) */
    std::vector<unsigned char> mDataParams; /** Raw buffer with all packed data params */

    bool mIsDirty = true; /** Dirty mechanism to track when to update params */
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_MATERIAL_HPP
