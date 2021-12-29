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

#include <core/io/Logger.hpp>
#include <render/material/Material.hpp>

#include <cassert>

BRK_NS_BEGIN

Material::Material(Ref<const Shader> shader) {
    assert(shader.IsNotNull());

    if (shader.IsNull()) {
        BRK_ERROR("Cannot create material from null shader object");
        return;
    }

    mShader = std::move(shader);
    mTechnique = mShader->FindFirstTechnique(ShaderTechniqueTags{}, true);
    assert(mTechnique.IsNotNull());

    if (mTechnique.IsNull()) {
        BRK_ERROR("Cannot find compiled technique for shader=" << mShader->GetName());
        return;
    }

    auto shaderParams = mShader->GetParams();
    assert(shaderParams.IsNotNull());

    mSamplers.resize(shaderParams->GetTexturesCount());
    mTextures.resize(shaderParams->GetTexturesCount());
    mDataParams = shaderParams->GetDefaultDataValues();
    assert(mDataParams.size() == shaderParams->GetDataSize());

    mPackedParams = Ref<MaterialParams>(new MaterialParams(*this));
}

void Material::SetName(StringName name) {
    mName = std::move(name);
}

void Material::SetDescription(String description) {
    mDescription = std::move(description);
}

void Material::SetTexture(const StringName &name, Ref<RHITexture> texture, Ref<RHISampler> sampler, uint32 arrayIndex) {
    assert(mShader.IsNotNull());
    assert(texture.IsNotNull());
    assert(sampler.IsNotNull());

    const auto &params = mShader->GetParams();
    auto *param = params->GetParamLight(name);

    if (!param || param->type != ShaderParamType::Texture) {
        BRK_ERROR("No such texture param to set param=" << name);
        return;
    }
    if (arrayIndex >= param->arraySize) {
        BRK_ERROR("Array index of param=" << name << " out of bounds (size=" << param->arraySize << " index=" << arrayIndex << ")");
        return;
    }

    const auto &paramInfo = params->GetTextureParamsInfo()[param->info];
    mSamplers[paramInfo.offset + arrayIndex] = std::move(sampler);
    mTextures[paramInfo.offset + arrayIndex] = std::move(texture);
    mIsDirty = true;
}

void Material::SetFloat1(const StringName &name, float value, uint32 arrayIndex) {
    SetDataParam(name, RHIShaderDataType::Float1, &value, arrayIndex);
}

void Material::SetFloat2(const StringName &name, const Vec2f &value, uint32 arrayIndex) {
    SetDataParam(name, RHIShaderDataType::Float2, &value, arrayIndex);
}

void Material::SetFloat3(const StringName &name, const Vec3f &value, uint32 arrayIndex) {
    SetDataParam(name, RHIShaderDataType::Float3, &value, arrayIndex);
}

void Material::SetFloat4(const StringName &name, const Vec4f &value, uint32 arrayIndex) {
    SetDataParam(name, RHIShaderDataType::Float4, &value, arrayIndex);
}

void Material::SetMat2(const StringName &name, const Mat2x2f &value, uint32 arrayIndex) {
    SetDataParam(name, RHIShaderDataType::Mat2, &value, arrayIndex);
}

void Material::SetMat3(const StringName &name, const Mat3x3f &value, uint32 arrayIndex) {
    SetDataParam(name, RHIShaderDataType::Mat3, &value, arrayIndex);
}

void Material::SetMat4(const StringName &name, const Mat4x4f &value, uint32 arrayIndex) {
    SetDataParam(name, RHIShaderDataType::Mat4, &value, arrayIndex);
}

void Material::UpdatePack() {
    if (mIsDirty) {
        mPackedParams->Update(*this);
        mIsDirty = false;
    }
}

void Material::SetDataParam(const StringName &name, RHIShaderDataType type, const void *data, uint32 arrayIndex) {
    assert(mShader.IsNotNull());
    const auto &params = mShader->GetParams();
    auto *param = params->GetParamLight(name);

    if (!param || param->type != ShaderParamType::Data) {
        BRK_ERROR("No such data param to set param=" << name);
        return;
    }
    if (param->typeData != type) {
        BRK_ERROR("Param has another type=" << static_cast<uint32>(param->typeData) << " passed=" << static_cast<uint32>(type));
        return;
    }
    if (arrayIndex >= param->arraySize) {
        BRK_ERROR("Array index of param=" << name << " out of bounds (size=" << param->arraySize << " index=" << arrayIndex << ")");
        return;
    }

    const auto &paramInfo = params->GetDataParamsInfo()[param->info];
    Memory::Copy(mDataParams.data() + paramInfo.offset + paramInfo.size * arrayIndex, data, paramInfo.size);
    mIsDirty = true;
}

BRK_NS_END