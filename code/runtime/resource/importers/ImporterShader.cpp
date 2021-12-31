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

#include <core/Engine.hpp>
#include <resource/ResShader.hpp>
#include <resource/importers/ImporterShader.hpp>

BRK_NS_BEGIN

ImporterShader::ImporterShader() {
    mExtensions.emplace_back("shader.xml");
}

Ref<ResourceImportOptions> ImporterShader::CreateDefaultOptions() const {
    return Ref<ResourceImportOptions>(new ResShaderImportOptions);
}

const std::vector<String> &ImporterShader::GetSupportedExtensions() const {
    return mExtensions;
}

void ImporterShader::Import(const String &fullpath, const Ref<ResourceImportOptions> &options, ResourceImportResult &result) {
    auto ops = options.Cast<ResShaderImportOptions>();
    Ref<ShaderCompileOptions> compileOptions;

    if (ops.IsNull()) {
        BRK_ERROR("Passed invalid options to import shader file=" << fullpath);
        compileOptions = Ref<ShaderCompileOptions>(new ShaderCompileOptions);
    } else
        compileOptions = ops->options;

    auto &shaderManager = Engine::Instance().GetRenderEngine().GetShaderManager();
    auto shader = shaderManager.Load(fullpath, compileOptions);

    if (shader.IsNull()) {
        result.failed = true;
        result.error = BRK_TEXT("Failed load shader");
        return;
    }

    auto &fileSystem = Engine::Instance().GetFileSystem();

    Ref<ResShader> resShader(new ResShader);
    resShader->SetName(StringName(fileSystem.GetFileName(fullpath, true)));
    resShader->CreateFromShader(std::move(shader));
    result.resource = resShader.As<Resource>();
}

BRK_NS_END