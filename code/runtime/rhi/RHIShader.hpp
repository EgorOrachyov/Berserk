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

#ifndef BERSERK_RHISHADER_HPP
#define BERSERK_RHISHADER_HPP

#include <core/Config.hpp>
#include <core/Data.hpp>
#include <core/Typedefs.hpp>
#include <core/io/Logger.hpp>
#include <core/string/StringName.hpp>
#include <rhi/RHIResource.hpp>

#include <functional>
#include <unordered_map>
#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup rhi
 * @{
 */

/**
 * @class RHIShaderMeta
 * @brief Reflection info about compiled RHI program
 */
class RHIShaderMeta : public RefCnt {
public:
    /** @brief Vertex shader inputs */
    struct InputAttribute {
        StringName name;
        uint16 location;
        RHIVertexElementType type;
    };

    /** @brief Object params like samplers, textures */
    struct ObjectParam {
        StringName name;
        uint16 location;
        uint16 arraySize;
        RHIShaderParamType type;
    };

    /** @brief Data param within uniform block */
    struct DataParam {
        StringName name;
        uint16 elementSize;
        uint16 arraySize;
        uint16 arrayStride;
        uint16 blockSlot;
        uint32 blockOffset;
        RHIShaderDataType type;
    };

    /** @brief Uniform block info */
    struct DataParamBlock {
        StringName name;
        uint32 slot;
        uint32 size;
    };

    inline uint32 GetSamplerLocation(const StringName &p) const {
        auto query = samplers.find(p);
        if (query == samplers.end()) {
            BRK_ERROR("Failed to find p=\"" << p << "\" in shader name=\"" << name << "\'");
            return 0xffffffff;
        }
        return query->second.location;
    }

    inline uint32 GetParamBlockSlot(const StringName &p) const {
        auto query = paramBlocks.find(p);
        if (query == paramBlocks.end()) {
            BRK_ERROR("Failed to find p=\"" << p << "\" in shader name=\"" << name << "\'");
            return 0xffffffff;
        }
        return query->second.slot;
    }

    StringName name;
    std::unordered_map<StringName, InputAttribute> inputs;
    std::unordered_map<StringName, DataParam> params;
    std::unordered_map<StringName, DataParamBlock> paramBlocks;
    std::unordered_map<StringName, ObjectParam> samplers;
};

/**
 * @class RHIShaderStageDesc
 * @brief Single shader stage
 */
struct RHIShaderStageDesc {
    RHIShaderType type;
    Ref<Data> sourceCode;
};

/**
 * @class RHIShaderDesc
 * @brief Shader creation descriptor. Allows create from byteCode
 */
struct RHIShaderDesc {
    Ref<Data> byteCode;
    StringName name;
    RHIShaderLanguage language;
    std::vector<RHIShaderStageDesc> stages;
    std::function<void(Ref<class RHIShader>)> callback;
};

/**
 * @class RHIShader
 * @brief Compiled and linked native shader
 */
class RHIShader : public RHIResource {
public:
    /** @brief Program may have different status (incorrect shaders mush not crash the application) */
    enum class Status : uint32 {
        /** @brief Program pending to be compiled */
        PendingCompilation = 0,
        /** @brief Program successfully compiled */
        Compiled = 1,
        /** @brief Failed to compile and link program */
        FailedCompile = 2
    };

    BRK_API ~RHIShader() override = default;

    /**
     * Check program status: whether the program was compiled and all stages linked by RHI.
     *
     * @note An attempt to bind not-compiled program will cause abort in debug mode.
     * @note It is safe to call this function from any thread.
     *
     * @return Compilation status of the program
     */
    BRK_API virtual Status GetCompilationStatus() const = 0;

    /**
     * Get compiler message about this program compilation.
     * If compilation finished with an error returns error description, otherwise nothing.
     *
     * @note It is safe to call this function from any thread.
     *
     * @return Error message about this program compilation
     */
    BRK_API virtual String GetCompilerMessage() const = 0;

    /**
     * Get compiled program meta information for params & buffers communication.
     *
     * @note For `PendingCompilation` or `FailedCompile` is not valid function call.
     * @note It is safe to call this function from any thread.
     *
     * @return Reference to program meta info if present
     */
    BRK_API virtual Ref<const RHIShaderMeta> GetShaderMeta() const = 0;

    /** @return Shader name for debugging */
    const StringName &GetShaderName() const { return mName; }

    /** @return Source code language type */
    RHIShaderLanguage GetLanguage() const { return mLanguage; }

    /** @return  Source code per stages */
    const std::vector<RHIShaderStageDesc> &GetStages() const { return mStages; }

protected:
    /** Shader name for debugging */
    StringName mName;

    /** Source code language type */
    RHIShaderLanguage mLanguage;

    /** Source code per stages */
    std::vector<RHIShaderStageDesc> mStages;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RHISHADER_HPP
