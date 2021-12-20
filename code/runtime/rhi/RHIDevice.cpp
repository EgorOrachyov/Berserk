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
#include <rhi/RHIDevice.hpp>

BRK_NS_BEGIN

const std::vector<RHITextureFormat> &RHIDevice::GetSupportedFormats() const {
    return mSupportedTextureFormats;
}

const std::vector<RHIShaderLanguage> &RHIDevice::GetSupportedShaderLanguages() const {
    return mSupportedShaderLanguages;
}

RHIType RHIDevice::GetDriverType() const {
    return mType;
}

const RHIDeviceCaps &RHIDevice::GetCaps() const {
    return mCaps;
}

const Mat4x4f &RHIDevice::GetClipMatrix() const {
    return mClipMatrix;
}

#define BRK_RENDER_THREAD_SETUP        \
    auto &engine = Engine::Instance(); \
    auto &rhit = engine.GetRHIThread();

void RHIDevice::UpdateVertexBuffer(const Ref<RHIVertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<Data> &data) {
    BRK_RENDER_THREAD_SETUP

    if (rhit.OnThread()) {
        auto cmd = GetCoreCommandList();
        cmd->UpdateVertexBuffer(buffer, byteOffset, byteSize, data);
        return;
    }

    rhit.EnqueueUpdate([=]() {
        auto cmd = GetCoreCommandList();
        cmd->UpdateVertexBuffer(buffer, byteOffset, byteSize, data);
    });
}

void RHIDevice::UpdateIndexBuffer(const Ref<RHIIndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<Data> &data) {
    BRK_RENDER_THREAD_SETUP

    if (rhit.OnThread()) {
        auto cmd = GetCoreCommandList();
        cmd->UpdateIndexBuffer(buffer, byteOffset, byteSize, data);
        return;
    }

    rhit.EnqueueUpdate([=]() {
        auto cmd = GetCoreCommandList();
        cmd->UpdateIndexBuffer(buffer, byteOffset, byteSize, data);
    });
}

void RHIDevice::UpdateUniformBuffer(const Ref<RHIUniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<Data> &data) {
    BRK_RENDER_THREAD_SETUP

    if (rhit.OnThread()) {
        auto cmd = GetCoreCommandList();
        cmd->UpdateUniformBuffer(buffer, byteOffset, byteSize, data);
        return;
    }

    rhit.EnqueueUpdate([=]() {
        auto cmd = GetCoreCommandList();
        cmd->UpdateUniformBuffer(buffer, byteOffset, byteSize, data);
    });
}

void RHIDevice::UpdateTexture2D(const Ref<RHITexture> &texture, uint32 mipLevel, const Rect2u &region, const Ref<Data> &data) {
    BRK_RENDER_THREAD_SETUP

    if (rhit.OnThread()) {
        auto cmd = GetCoreCommandList();
        cmd->UpdateTexture2D(texture, mipLevel, region, data);
        return;
    }

    rhit.EnqueueUpdate([=]() {
        auto cmd = GetCoreCommandList();
        cmd->UpdateTexture2D(texture, mipLevel, region, data);
    });
}

void RHIDevice::UpdateTexture2DArray(const Ref<RHITexture> &texture, uint32 arrayIndex, uint32 mipLevel, const Rect2u &region, const Ref<Data> &data) {
    BRK_RENDER_THREAD_SETUP

    if (rhit.OnThread()) {
        auto cmd = GetCoreCommandList();
        cmd->UpdateTexture2DArray(texture, arrayIndex, mipLevel, region, data);
        return;
    }

    rhit.EnqueueUpdate([=]() {
        auto cmd = GetCoreCommandList();
        cmd->UpdateTexture2DArray(texture, arrayIndex, mipLevel, region, data);
    });
}

void RHIDevice::UpdateTextureCube(const Ref<RHITexture> &texture, RHITextureCubemapFace face, uint32 mipLevel, const Rect2u &region, const Ref<Data> &data) {
    BRK_RENDER_THREAD_SETUP

    if (rhit.OnThread()) {
        auto cmd = GetCoreCommandList();
        cmd->UpdateTextureCube(texture, face, mipLevel, region, data);
        return;
    }

    rhit.EnqueueUpdate([=]() {
        auto cmd = GetCoreCommandList();
        cmd->UpdateTextureCube(texture, face, mipLevel, region, data);
    });
}

void RHIDevice::UpdateResourceSet(const Ref<RHIResourceSet> &set, const RHIResourceSetDesc &desc) {
    BRK_RENDER_THREAD_SETUP

    if (rhit.OnThread()) {
        auto cmd = GetCoreCommandList();
        cmd->UpdateResourceSet(set, desc);
        return;
    }

    rhit.EnqueueUpdate([=]() {
        auto cmd = GetCoreCommandList();
        cmd->UpdateResourceSet(set, desc);
    });
}

void RHIDevice::GenerateMipMaps(const Ref<RHITexture> &texture) {
    BRK_RENDER_THREAD_SETUP

    if (rhit.OnThread()) {
        auto cmd = GetCoreCommandList();
        cmd->GenerateMipMaps(texture);
        return;
    }

    rhit.EnqueueUpdate([=]() {
        auto cmd = GetCoreCommandList();
        cmd->GenerateMipMaps(texture);
    });
}

#undef BRK_RENDER_THREAD_SETUP

BRK_NS_END
