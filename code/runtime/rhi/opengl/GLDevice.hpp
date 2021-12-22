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

#ifndef BERSERK_GLDEVICE_HPP
#define BERSERK_GLDEVICE_HPP

#include <core/Thread.hpp>
#include <rhi/RHIDevice.hpp>
#include <rhi/opengl/GLDefs.hpp>

#include <functional>

BRK_NS_BEGIN

/**
 * @addtogroup opengl
 * @{
 */

/**
 * @class GLDevice
 * @brief GL device implementation
 */
class GLDevice final : public RHIDevice {
public:
    using MakeContextCurrentFunc = std::function<void(const Ref<Window> &)>;
    using SwapBuffersFunc = std::function<void(const Ref<Window> &)>;

    BRK_API explicit GLDevice(MakeContextCurrentFunc makeCurrentFunc, SwapBuffersFunc swapBuffersFunc);
    BRK_API ~GLDevice() override;

    BRK_API Ref<RHIVertexDeclaration> CreateVertexDeclaration(const RHIVertexDeclarationDesc &desc) override;
    BRK_API Ref<RHIVertexBuffer> CreateVertexBuffer(const RHIBufferDesc &desc) override;
    BRK_API Ref<RHIIndexBuffer> CreateIndexBuffer(const RHIBufferDesc &desc) override;
    BRK_API Ref<RHIUniformBuffer> CreateUniformBuffer(const RHIBufferDesc &desc) override;
    BRK_API Ref<RHISampler> CreateSampler(const RHISamplerDesc &desc) override;
    BRK_API Ref<RHITexture> CreateTexture(const RHITextureDesc &desc) override;
    BRK_API Ref<RHIResourceSet> CreateResourceSet(const RHIResourceSetDesc &desc) override;
    BRK_API Ref<RHIFramebuffer> CreateFramebuffer(const RHIFramebufferDesc &desc) override;
    BRK_API Ref<RHIShader> CreateShader(const RHIShaderDesc &desc) override;
    BRK_API Ref<RHIRenderPass> CreateRenderPass(const RHIRenderPassDesc &desc) override;
    BRK_API Ref<RHIGraphicsPipeline> CreateGraphicsPipeline(const RHIGraphicsPipelineDesc &desc) override;
    BRK_API Ref<RHICommandList> GetCoreCommandList() override;

    BRK_API MakeContextCurrentFunc &GetContextFunc();
    BRK_API SwapBuffersFunc &GetSwapFunc();

    /**
     * @brief Create GL RHI device
     *
     * Attempts to load and initialize GL functions
     * and create GL RHI device.
     *
     * @param makeCurrentFunc Function to bind context of the window
     * @param swapBuffersFunc Function to swap buffers to present window image
     *
     * @return Created device; null if failed
     */
    BRK_API static std::shared_ptr<GLDevice> Make(MakeContextCurrentFunc makeCurrentFunc, SwapBuffersFunc swapBuffersFunc);

private:
    MakeContextCurrentFunc mMakeCurrentFunc;
    SwapBuffersFunc mSwapBuffersFunc;
    Thread *mRHIThread = nullptr;
    Ref<class GLCommandList> mCoreCommandList;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_GLDEVICE_HPP
