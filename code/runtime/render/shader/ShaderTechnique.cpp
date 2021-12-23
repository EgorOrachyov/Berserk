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

#include <render/shader/ShaderTechnique.hpp>

BRK_NS_BEGIN

void ShaderTechnique::SetName(StringName name) {
    mName = std::move(name);
}

void ShaderTechnique::SetArchetype(StringName archetype) {
    mArchetype = std::move(archetype);
}

void ShaderTechnique::SetTags(const ShaderTechniqueTags &tags) {
    mTags = tags;
}

void ShaderTechnique::SetRHIType(RHIType rhiType) {
    mRHIType = rhiType;
}

void ShaderTechnique::SetRHILanguage(RHIShaderLanguage language) {
    mRHILanguage = language;
}

void ShaderTechnique::SetPasses(std::vector<Ref<ShaderPass>> passes) {
    mIsCompiled = false;
    mPasses = std::move(passes);
}

void ShaderTechnique::SetShader(class Shader *shader) {
    mShader = shader;
}

void ShaderTechnique::FillPipelineDesc(RHIGraphicsPipelineDesc &desc, uint32 passIdx) const {
    assert(passIdx < mPasses.size());
    desc.declaration = mVertexDeclaration;
    mPasses[passIdx]->FillPipelineDesc(desc);
}

bool ShaderTechnique::IsCompiled() const {
    // If not compiled, check state
    if (!mIsCompiled) {
        mIsCompiled = true;
        std::for_each(mPasses.begin(), mPasses.end(),
                      [&](const Ref<ShaderPass> &p) { mIsCompiled = mIsCompiled && p->IsCompiled(); });
    }
    return mIsCompiled;
}

Ref<const ShaderPass> ShaderTechnique::GetPass(const StringName &name) const {
    for (const auto &p : mPasses)
        if (p->GetName() == name)
            return p.As<const ShaderPass>();
    return Ref<const ShaderPass>();
}

Ref<const ShaderPass> ShaderTechnique::GetPass(uint32 passIdx) const {
    assert(passIdx < mPasses.size());
    if (passIdx < mPasses.size())
        return mPasses[passIdx].As<const ShaderPass>();
    return Ref<const ShaderPass>();
}

BRK_NS_END