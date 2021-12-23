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

#include <render/shader/Shader.hpp>

BRK_NS_BEGIN

void Shader::SetName(StringName name) {
    mName = std::move(name);
}

void Shader::SetLoadPath(String loadPath) {
    mLoadPath = std::move(loadPath);
}

void Shader::SetVariation(ShaderVariation variation) {
    mVariation = variation;
}

void Shader::SetOptions(ShaderCompileOptions options) {
    mOptions = std::move(options);
}

void Shader::SetTechniques(std::vector<Ref<ShaderTechnique>> techniques) {
    mTechniques = std::move(techniques);
}

void Shader::SetParams(Ref<ShaderParams> params) {
    mParams = std::move(params);
}

Ref<const ShaderTechnique> Shader::GetTechnique(const StringName &name) const {
    for (const auto &technique : mTechniques)
        if (technique->GetName() == name)
            return technique.As<const ShaderTechnique>();
    return Ref<const ShaderTechnique>();
}

Ref<const ShaderTechnique> Shader::GetTechnique(uint32 id) const {
    assert(id < GetTechniquesCount());
    return mTechniques[id].As<const ShaderTechnique>();
}

Ref<const ShaderTechnique> Shader::FindFirstTechnique(const ShaderTechniqueTags &tags, bool mustBeCompiled) const {
    for (const auto &technique : mTechniques) {
        const auto &tTags = technique->GetTags();

        if ((tTags.value & tags.value) == tags.value && (!mustBeCompiled || technique->IsCompiled()))
            return technique.As<const ShaderTechnique>();
    }
    return Ref<const ShaderTechnique>();
}

BRK_NS_END