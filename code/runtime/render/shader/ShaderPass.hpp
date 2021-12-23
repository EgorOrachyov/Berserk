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

#ifndef BERSERK_SHADERPASS_HPP
#define BERSERK_SHADERPASS_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/string/StringName.hpp>
#include <rhi/RHIGraphicsPipeline.hpp>
#include <rhi/RHIShader.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup render
 * @{
 */

/**
 * @class ShaderPass
 * @brief Single rendering pass stage of the shader
 */
class ShaderPass final : public RefCnt {
public:
    /** Construct empty render pass */
    BRK_API ShaderPass() = default;
    BRK_API ~ShaderPass() override = default;

    /** Set pass name within technique */
    BRK_API void SetName(StringName name);
    /** Set pass shader program */
    BRK_API void SetShader(Ref<RHIShader> shader);
    /** Set pass depth stencil state */
    BRK_API void SetDepthStencil(const RHIDepthStencilState &state);
    /** Set pass raster state */
    BRK_API void SetRasterState(const RHIRasterState &state);
    /** Set pass blend state */
    BRK_API void SetBlendState(RHIBlendState state);
    /** Set technique this pass belongs to */
    BRK_API void SetTechnique(class ShaderTechnique *technique);

    /** Fill pipeline descriptor */
    BRK_API void FillPipelineDesc(RHIGraphicsPipelineDesc &desc) const;

    /** @return True if pass compiled */
    BRK_API bool IsCompiled() const;

    /** @return Pass name within technique */
    BRK_API const StringName &GetName() const { return mName; }
    /** @return Pass shader program */
    BRK_API const Ref<RHIShader> &GetShader() const { return mShader; }
    /** @return Pass depth stencil state */
    BRK_API const RHIDepthStencilState &GetDepthStencil() const { return mDepthStencil; }
    /** @return Pass raster state */
    BRK_API const RHIRasterState &GetRasterState() const { return mRasterState; }
    /** @return Pass blend state */
    BRK_API const RHIBlendState &GetBlendState() const { return mBlendState; }
    /** @return Technique this pass belongs to */
    BRK_API class ShaderTechnique *GetTechnique() const { return mTechnique; }

private:
    /** Name for clarity */
    StringName mName;
    /** Pipeline setup */
    Ref<RHIShader> mShader{};
    RHIDepthStencilState mDepthStencil{};
    RHIRasterState mRasterState{};
    RHIBlendState mBlendState{};
    /** Parent technique this pass belongs to  */
    class ShaderTechnique *mTechnique = nullptr;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_SHADERPASS_HPP
