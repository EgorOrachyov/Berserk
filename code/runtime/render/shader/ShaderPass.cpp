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
#include <render/shader/ShaderPass.hpp>

BRK_NS_BEGIN

void ShaderPass::SetName(StringName name) {
    mName = std::move(name);
}

void ShaderPass::SetShader(Ref<RHIShader> shader) {
    mShader = std::move(shader);
}

void ShaderPass::SetTechnique(class ShaderTechnique *technique) {
    mTechnique = technique;
}

void ShaderPass::FillPipelineDesc(RHIGraphicsPipelineDesc &desc) const {
#ifdef BERSERK_DEBUG
    if (!IsCompiled()) {
        BRK_WARNING("An attempt to fill graphics pipeline desc"
                    " from not compiled shader pass name="
                    << GetName());
    }
#endif
    desc.shader = mShader;
}

bool ShaderPass::IsCompiled() const {
    return mShader.IsNotNull() && mShader->GetCompilationStatus() == RHIShader::Status::Compiled;
}

BRK_NS_END