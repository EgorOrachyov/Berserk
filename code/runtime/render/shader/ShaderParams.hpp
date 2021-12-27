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

#ifndef BERSERK_SHADERPARAMS_HPP
#define BERSERK_SHADERPARAMS_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/string/StringName.hpp>
#include <rhi/RHIDefs.hpp>

#include <unordered_map>
#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup render
 * @{
 */

/**
 * @class ShaderParamType
 * @brief General types of shader params
 */
enum class ShaderParamType : uint8 {
    /** Texture resource (some of RHI texture 2d, array, cube and etc.)*/
    Texture,
    /** Raw data value (some of vec of int, float values and etc.) */
    Data,
    /** Unspecified */
    Unknown
};

/**
 * @class ShaderParam
 * @brief Describes single param in the shader
 */
struct ShaderParam {
    /** Name of the param to set in material */
    StringName name;
    /** Optional param name displayed for the user */
    StringName uiName;
    /** Optional param description */
    String description;
    /** Optional default param value */
    String defaultValue;
    /** Size of array (if more than 1 value within a param) */
    uint32 arraySize = 1;
    /** [Internal] */
    uint32 info = 0xffffffff;
    /** Type of param */
    ShaderParamType type = ShaderParamType::Unknown;
    /** [Internal] RHI type of variable */
    RHIShaderDataType typeData = RHIShaderDataType::Unknown;
    /** [Internal] RHI type of variable */
    RHIShaderParamType typeParam = RHIShaderParamType::Unknown;
};

/**
 * @class ShaderParams
 * @brief Describes layout of shader params for packing by material
 */
class ShaderParams final : public RefCnt {
public:
    static const uint32 INVALID_ID = 0xffffffff;

    /** Reduced param info for fast look-up */
    struct ShaderParamLight {
        uint32 info = 0xffffffff;
        uint32 arraySize = 0u;
        ShaderParamType type = ShaderParamType::Unknown;
        RHIShaderDataType typeData = RHIShaderDataType::Unknown;
        RHIShaderParamType typeParam = RHIShaderParamType::Unknown;
    };

    /** @brief Info about raw data param */
    struct DataParamInfo {
        uint32 offset = 0;
        uint32 size = 0;
    };

    /** @brief Info about texture data param */
    struct TextureParamInfo {
        uint32 offset = 0;
    };

    /** Create params layout from list of params */
    BRK_API explicit ShaderParams(std::vector<ShaderParam> params);
    BRK_API ~ShaderParams() override = default;

    /** @return Check if has param */
    BRK_API bool HasParam(const StringName &name) const;

    BRK_API uint32 GetParamId(const StringName &name) const;
    BRK_API const ShaderParam *GetParam(const StringName &name) const;
    BRK_API const ShaderParam &GetParam(uint32 id) const;

    /** @return Look-up info */
    BRK_API const std::unordered_map<StringName, uint32> &GetParamLookUp() const { return mParamLookUp; }
    /** @return Params list */
    BRK_API const std::vector<ShaderParam> &GetParams() const { return mParams; }
    /** @return Params list */
    BRK_API const std::vector<ShaderParamLight> &GetParamsLight() const { return mParamsLight; }
    /** @return Data params info */
    BRK_API const std::vector<DataParamInfo> &GetDataParamsInfo() const { return mDataParamsInfo; }
    /** @return Texture params info */
    BRK_API const std::vector<TextureParamInfo> &GetTextureParamsInfo() const { return mTextureParamsInfo; }

    /** Byte size of buffer required to pack all data params */
    uint32 GetDataSize() const { return mDataSize; }
    /** Total number of textures (includes array size factor) */
    uint32 GetTexturesCount() const { return mTexturesCount; }

protected:
    /** Define params layout and build look-up structure */
    void Build();
    /** Define default values */
    void InitDefaults();

private:
    std::unordered_map<StringName, uint32> mParamLookUp; /** Find location of param by public name */
    std::vector<ShaderParam> mParams;                    /** Params list */
    std::vector<ShaderParamLight> mParamsLight;          /** Fast look-up (reduced param info) */
    std::vector<DataParamInfo> mDataParamsInfo;          /** Data params info */
    std::vector<TextureParamInfo> mTextureParamsInfo;    /** Texture params info */
    std::vector<unsigned char> mDefaultDataValues;       /** [Internal] Default data values */
    uint32 mDataSize = 0;                                /** Byte size of buffer required to pack all data params */
    uint32 mTexturesCount = 0;                           /** Total number of textures (includes array size factor) */
};

BRK_NS_END

#endif//BERSERK_SHADERPARAMS_HPP
