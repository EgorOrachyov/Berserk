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
#include <render/shader/ShaderParams.hpp>

#include <cassert>

BRK_NS_BEGIN

ShaderParams::ShaderParams(std::vector<ShaderParam> params) : mParams(std::move(params)) {
    Build();
}

bool ShaderParams::HasParam(const StringName &name) const {
    return GetParamId(name) != INVALID_ID;
}

uint32 ShaderParams::GetParamId(const StringName &name) const {
    auto query = mParamLookUp.find(name);
    return query != mParamLookUp.end() ? query->second : INVALID_ID;
}

const ShaderParam *ShaderParams::GetParam(const StringName &name) const {
    auto query = mParamLookUp.find(name);
    return query != mParamLookUp.end() ? &GetParam(query->second) : nullptr;
}

const ShaderParam &ShaderParams::GetParam(uint32 id) const {
    assert(id < mParams.size());
    return mParams[id];
}

void ShaderParams::Build() {
    uint32 nextDataInfo = 0;
    uint32 nextTextureInfo = 0;
    uint32 nextDataOffset = 0;
    uint32 nextTextureLocation = 0;

    for (uint32 id = 0; id < static_cast<uint32>(mParams.size()); id++) {
        auto &param = mParams[id];

        assert(param.arraySize > 0);
        assert(param.type != ShaderParamType::Unknown);

        mParamLookUp.emplace(param.name, id);

        switch (param.type) {

            case ShaderParamType::Data:
                assert(param.typeData != RHIShaderDataType::Unknown);
                mDataParamsInfo.emplace_back();
                mDataParamsInfo[nextDataInfo].size = GetRHIShaderDataTypeSize(param.typeData);
                mDataParamsInfo[nextDataInfo].offset = nextDataOffset;
                param.id = nextDataInfo;
                nextDataOffset += mDataParamsInfo[nextDataInfo].size * param.arraySize;
                nextDataInfo += 1;
                break;

            case ShaderParamType::Texture:
                assert(param.typeParam != RHIShaderParamType::Unknown);
                mTextureParamsInfo.emplace_back();
                mTextureParamsInfo[nextTextureInfo].offset = nextTextureLocation;
                param.id = nextTextureInfo;
                nextTextureLocation += param.arraySize;
                nextTextureInfo += 1;
                break;

            default:
                BRK_ERROR("Shader param name=\"" << param.name << "\" has Unknown type. Skip");
                continue;
        }
    }

    mDataSize = nextDataOffset;
    mTexturesCount = nextTextureLocation;
}

BRK_NS_END
