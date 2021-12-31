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

#include <resource/ResShader.hpp>

BRK_NS_BEGIN

const StringName &ResShader::GetResourceType() const {
    return GetResourceTypeStatic();
}

const StringName &ResShader::GetResourceTypeStatic() {
    static StringName resourceType(BRK_TEXT("_brk_resource_shader"));
    return resourceType;
}

void ResShader::CreateFromShader(Ref<const Shader> shader) {
    if (shader.IsNull()) {
        BRK_ERROR("An attempt to create shader resource from null shader");
        return;
    }

    mShader = std::move(shader);
}

bool ResShader::HasParam(const StringName &name) const {
    assert(mShader.IsNotNull());
    return mShader->GetParams()->HasParam(name);
}

const StringName &ResShader::GetArchetype() const {
    assert(mShader.IsNotNull());
    return mShader->GetArchetype();
}

const Ref<ShaderCompileOptions> &ResShader::GetCompileOptions() const {
    assert(mShader.IsNotNull());
    return mShader->GetOptions();
}

BRK_NS_END