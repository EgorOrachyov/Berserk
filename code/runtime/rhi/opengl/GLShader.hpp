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

#ifndef BERSERK_GLSHADER_HPP
#define BERSERK_GLSHADER_HPP

#include <rhi/RHIShader.hpp>
#include <rhi/opengl/GLDefs.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup opengl
 * @{
 */

/**
 * @class GLShader
 * @brief GL Shader (compiled and linked) native program
 */
class GLShader final : public RHIShader {
public:
    BRK_API explicit GLShader(const RHIShaderDesc &desc);
    BRK_API ~GLShader() override;

    BRK_API Status GetCompilationStatus() const override;
    BRK_API String GetCompilerMessage() const override;
    BRK_API Ref<const RHIShaderMeta> GetShaderMeta() const override;

    BRK_API void Initialize();
    BRK_API bool ValidateStages() const;
    BRK_API void InitializeMeta();
    BRK_API void BindUniformBlock(uint32 binding) const;
    BRK_API void Use() const;

    GLuint GetHandle() const { return mHandle; }

private:
    GLuint mHandle = 0;
    String mCompilerMessage;
    Ref<RHIShaderMeta> mMeta{};
    Ref<Data> mByteCode{};
    std::atomic<Status> mCompilationStatus{Status::PendingCompilation};
    std::function<void(Ref<class RHIShader>)> mCallback;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_GLSHADER_HPP
