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

#ifndef BERSERK_RESSHADER_HPP
#define BERSERK_RESSHADER_HPP

#include <resource/Resource.hpp>
#include <resource/ResourceImporter.hpp>

#include <render/shader/Shader.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup resource
 * @{
 */

/**
 * @class ResShaderImportOptions
 * @brief Options to import shader resource
 */
class ResShaderImportOptions final : public ResourceImportOptions {
public:
    BRK_API ~ResShaderImportOptions() override = default;

    Ref<ShaderCompileOptions> options{new ShaderCompileOptions}; /** Options to compile shader */
};

/**
 * @class ResShader
 * @brief Engine shader program for rendering
 */
class ResShader final : public Resource {
public:
    BRK_API ResShader() = default;
    BRK_API ~ResShader() override = default;

    BRK_API const StringName &GetResourceType() const override;
    BRK_API static const StringName &GetResourceTypeStatic();

    BRK_API void CreateFromShader(Ref<const Shader> shader);
    BRK_API bool HasParam(const StringName &name) const;

    BRK_API const StringName &GetArchetype() const;
    BRK_API const Ref<ShaderCompileOptions> &GetCompileOptions() const;
    BRK_API const Ref<const Shader> &GetShader() const { return mShader; }

private:
    Ref<const Shader> mShader; /** Actual shader program */
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RESSHADER_HPP
