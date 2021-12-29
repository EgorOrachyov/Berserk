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

#include <core/Memory.hpp>
#include <core/io/Logger.hpp>
#include <render/shader/ShaderParams.hpp>

#include <cassert>
#include <sstream>

BRK_NS_BEGIN

ShaderParams::ShaderParams(std::vector<ShaderParam> params) : mParams(std::move(params)) {
    Build();
    InitDefaults();
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

const ShaderParams::ShaderParamLight *ShaderParams::GetParamLight(const StringName &name) const {
    auto query = mParamLookUp.find(name);
    return query != mParamLookUp.end() ? &GetParamLight(query->second) : nullptr;
}

const ShaderParam &ShaderParams::GetParam(uint32 id) const {
    assert(id < mParams.size());
    return mParams[id];
}

const ShaderParams::ShaderParamLight &ShaderParams::GetParamLight(uint32 id) const {
    assert(id < mParamsLight.size());
    return mParamsLight[id];
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
                param.info = nextDataInfo;
                nextDataOffset += mDataParamsInfo[nextDataInfo].size * param.arraySize;
                nextDataInfo += 1;
                break;

            case ShaderParamType::Texture:
                assert(param.typeParam != RHIShaderParamType::Unknown);
                mTextureParamsInfo.emplace_back();
                mTextureParamsInfo[nextTextureInfo].offset = nextTextureLocation;
                param.info = nextTextureInfo;
                nextTextureLocation += param.arraySize;
                nextTextureInfo += 1;
                break;

            default:
                BRK_ERROR("Shader param name=\"" << param.name << "\" has Unknown type. Skip");
                continue;
        }
    }

    mParamsLight.resize(mParams.size());
    for (uint32 id = 0; id < static_cast<uint32>(mParams.size()); id++) {
        const auto &param = mParams[id];
        auto &paramLight = mParamsLight[id];

        paramLight.info = param.info;
        paramLight.arraySize = param.arraySize;
        paramLight.type = param.type;
        paramLight.typeData = param.typeData;
        paramLight.typeParam = param.typeParam;
    }

    mDataSize = nextDataOffset;
    mTexturesCount = nextTextureLocation;
}

void ShaderParams::InitDefaults() {
    mDefaultDataValues.resize(GetDataSize(), 0x0);

    for (const auto &param : GetParams()) {
        if (param.type == ShaderParamType::Data) {
            const auto &defaultValue = param.defaultValue;

            // Parse value accordingly to type of param
            if (!defaultValue.empty()) {
                auto type = param.typeData;
                auto &paramInfo = mDataParamsInfo[param.info];
                auto offset = paramInfo.offset;

                std::stringstream s(defaultValue);

                const uint32 MAX_VALUES = 4 * 4;

                int valuesInt[MAX_VALUES];
                float valuesFloat[MAX_VALUES];
                unsigned int valuesUint[MAX_VALUES];

                uint32 next = 0;
                uint32 toRead = RHIGetBaseElementsCount(type);

                assert(toRead < MAX_VALUES);

                // Initialize first value
                if (RHIIsFloatBaseType(type)) {
                    for (uint32 c = 0; c < toRead; c++) {
                        s >> valuesFloat[next];
                        next += 1;
                    }
                    Memory::Copy(mDefaultDataValues.data() + offset, valuesFloat, sizeof(float) * toRead);
                } else if (RHIIsIntegralBaseSignedType(type)) {
                    for (uint32 c = 0; c < toRead; c++) {
                        s >> valuesInt[next];
                        next += 1;
                    }
                    Memory::Copy(mDefaultDataValues.data() + offset, valuesInt, sizeof(int) * toRead);
                } else {
                    for (uint32 c = 0; c < toRead; c++) {
                        s >> valuesUint[next];
                        next += 1;
                    }
                    Memory::Copy(mDefaultDataValues.data() + offset, valuesUint, sizeof(unsigned int) * toRead);
                }

                // If array, copy to all values v[0] -> v[1] .. v[n-1]
                auto arraySize = param.arraySize;
                auto size = paramInfo.size;
                for (uint32 i = 1; i < arraySize; i++) {
                    Memory::Copy(mDefaultDataValues.data() + offset + i * size, mDefaultDataValues.data() + offset, size);
                }
            }
        }
    }
}

BRK_NS_END
