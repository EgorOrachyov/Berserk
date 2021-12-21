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

#include <core/image/ImageUtil.hpp>
#include <rhi/opengl/GLTexture.hpp>

BRK_NS_BEGIN

GLTexture::GLTexture(const RHITextureDesc &desc) {
    mDesc = desc;
}

GLTexture::~GLTexture() {
    if (mHandle) {
        glDeleteTextures(1, &mHandle);
        mHandle = 0;
    }
}

void GLTexture::Initialize() {
    switch (mDesc.textureType) {
        case RHITextureType::Texture2d:
            Initialize2d();
            break;
        case RHITextureType::Texture2dArray:
            Initialize2dArray();
            break;
        case RHITextureType::TextureCube:
            InitializeCube();
            break;
        default:
            BRK_ERROR("Unsupported RHITextureType");
            break;
    }
}

void GLTexture::Initialize2d() {
    assert(GetWidth() > 0);
    assert(GetHeight() > 0);
    assert(GetDepth() == 1);
    assert(GetArraySlices() == 1);

    auto target = GetTextureTarget();
    auto internalFormat = GLDefs::GetTextureInternalFormat(GetTextureFormat());
    auto mipsCount = GetMipsCount();

    assert(mipsCount >= 1);
    assert(mipsCount <= ImageUtil::GetMaxMipsCount(GetWidth(), GetHeight(), GetDepth()));

    glGenTextures(1, &mHandle);
    BRK_GL_CATCH_ERR();

    glBindTexture(target, mHandle);
    BRK_GL_CATCH_ERR();

    auto width = GetWidth();
    auto height = GetHeight();

    auto dataFormat = GLDefs::GetTextureDataBaseFormat(GetTextureFormat());
    auto dataType = GLDefs::GetTextureDataType(GetTextureFormat());

    for (GLuint level = 0; level < mipsCount; level++) {
        glTexImage2D(target, level, internalFormat, width, height, 0, dataFormat, dataType, nullptr);
        BRK_GL_CATCH_ERR();

        if (width > 1) width /= 2;
        if (height > 1) height /= 2;
    }

    glBindTexture(target, 0);
    BRK_GL_CATCH_ERR();
}

void GLTexture::Initialize2dArray() {
    assert(GetWidth() > 0);
    assert(GetHeight() > 0);
    assert(GetDepth() == 1);
    assert(GetArraySlices() > 0);

    auto target = GetTextureTarget();
    auto internalFormat = GLDefs::GetTextureInternalFormat(GetTextureFormat());
    auto mipsCount = GetMipsCount();

    assert(mipsCount >= 1);
    assert(mipsCount <= ImageUtil::GetMaxMipsCount(GetWidth(), GetHeight(), GetDepth()));

    glGenTextures(1, &mHandle);
    BRK_GL_CATCH_ERR();

    glBindTexture(target, mHandle);
    BRK_GL_CATCH_ERR();

    auto width = GetWidth();
    auto height = GetHeight();
    auto arraySize = GetArraySlices();

    auto dataFormat = GLDefs::GetTextureDataBaseFormat(GetTextureFormat());
    auto dataType = GLDefs::GetTextureDataType(GetTextureFormat());

    for (GLuint level = 0; level < mipsCount; level++) {
        glTexImage3D(target, level, internalFormat, width, height, arraySize, 0, dataFormat, dataType, nullptr);
        BRK_GL_CATCH_ERR();

        if (width > 1) width /= 2;
        if (height > 1) height /= 2;
    }

    glBindTexture(target, 0);
    BRK_GL_CATCH_ERR();
}

void GLTexture::InitializeCube() {
    assert(GetWidth() > 0);
    assert(GetHeight() > 0);
    assert(GetDepth() == 1);
    assert(GetArraySlices() == 1);

    auto target = GetTextureTarget();
    auto internalFormat = GLDefs::GetTextureInternalFormat(GetTextureFormat());
    auto mipsCount = GetMipsCount();

    assert(mipsCount >= 1);
    assert(mipsCount <= ImageUtil::GetMaxMipsCount(GetWidth(), GetHeight(), GetDepth()));

    glGenTextures(1, &mHandle);
    BRK_GL_CATCH_ERR();

    glBindTexture(target, mHandle);
    BRK_GL_CATCH_ERR();

    auto width = GetWidth();
    auto height = GetHeight();

    auto dataFormat = GLDefs::GetTextureDataBaseFormat(GetTextureFormat());
    auto dataType = GLDefs::GetTextureDataType(GetTextureFormat());

    static const RHITextureCubemapFace faces[RHILimits::MAX_TEXTURE_CUBE_FACES] = {
            RHITextureCubemapFace::PositiveX,
            RHITextureCubemapFace::NegativeX,
            RHITextureCubemapFace::PositiveY,
            RHITextureCubemapFace::NegativeY,
            RHITextureCubemapFace::PositiveZ,
            RHITextureCubemapFace::NegativeZ};

    for (auto face : faces) {
        auto faceTarget = GLDefs::GetTextureCubeFaceTarget(face);

        auto w = width;
        auto h = height;

        for (GLuint level = 0; level < mipsCount; level++) {
            glTexImage2D(faceTarget, level, internalFormat, w, h, 0, dataFormat, dataType, nullptr);
            BRK_GL_CATCH_ERR();

            if (w > 1) w /= 2;
            if (h > 1) h /= 2;
        }
    }

    glBindTexture(target, 0);
    BRK_GL_CATCH_ERR();
}

void GLTexture::UpdateTexture2D(uint32 mipLevel, const Rect2u &region, const Ref<Data> &memory) {
    assert(RHITextureType::Texture2d == GetTextureType());
    assert(mipLevel < GetMipsCount());
    assert(region.x() <= region.z());
    assert(region.y() <= region.w());

    auto mipSize = ImageUtil::GetMipSize(mipLevel, GetWidth(), GetHeight());
    assert(region.z() <= mipSize.x());
    assert(region.w() <= mipSize.y());

    const auto *pixels = memory->GetData();
    auto dataFormat = GLDefs::GetTextureDataBaseFormat(GetTextureFormat());
    auto dataType = GLDefs::GetTextureDataType(GetTextureFormat());
    auto target = GetTextureTarget();

    auto xoffset = static_cast<GLint>(region.x());
    auto yoffset = static_cast<GLint>(region.y());
    auto width = static_cast<GLsizei>(region.z());
    auto height = static_cast<GLsizei>(region.w());

    glBindTexture(target, mHandle);
    BRK_GL_CATCH_ERR();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    BRK_GL_CATCH_ERR();

    glTexSubImage2D(target, mipLevel, xoffset, yoffset, width, height, dataFormat, dataType, pixels);
    BRK_GL_CATCH_ERR();

    glBindTexture(target, 0);
    BRK_GL_CATCH_ERR();
}

void GLTexture::UpdateTexture2DArray(uint32 arrayIndex, uint32 mipLevel, const Rect2u &region, const Ref<Data> &memory) {
    assert(RHITextureType::Texture2dArray == GetTextureType());
    assert(arrayIndex < GetArraySlices());
    assert(mipLevel < GetMipsCount());
    assert(region.x() <= region.z());
    assert(region.y() <= region.w());

    auto mipSize = ImageUtil::GetMipSize(mipLevel, GetWidth(), GetHeight());
    assert(region.z() <= mipSize.x());
    assert(region.w() <= mipSize.y());

    const auto *pixels = memory->GetData();
    auto dataFormat = GLDefs::GetTextureDataBaseFormat(GetTextureFormat());
    auto dataType = GLDefs::GetTextureDataType(GetTextureFormat());
    auto target = GetTextureTarget();

    auto xoffset = static_cast<GLint>(region.x());
    auto yoffset = static_cast<GLint>(region.y());
    auto width = static_cast<GLsizei>(region.z());
    auto height = static_cast<GLsizei>(region.w());

    glBindTexture(target, mHandle);
    BRK_GL_CATCH_ERR();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    BRK_GL_CATCH_ERR();

    glTexSubImage3D(target, mipLevel, xoffset, yoffset, arrayIndex, width, height, 1, dataFormat, dataType, pixels);
    BRK_GL_CATCH_ERR();

    glBindTexture(target, 0);
    BRK_GL_CATCH_ERR();
}

void GLTexture::UpdateTextureCube(RHITextureCubemapFace face, uint32 mipLevel, const Rect2u &region, const Ref<Data> &memory) {
    assert(RHITextureType::TextureCube == GetTextureType());
    assert(mipLevel < GetMipsCount());
    assert(region.x() <= region.z());
    assert(region.y() <= region.w());

    auto mipSize = ImageUtil::GetMipSize(mipLevel, GetWidth(), GetHeight());
    assert(region.z() <= mipSize.x());
    assert(region.w() <= mipSize.y());

    const auto *pixels = memory->GetData();
    auto dataFormat = GLDefs::GetTextureDataBaseFormat(GetTextureFormat());
    auto dataType = GLDefs::GetTextureDataType(GetTextureFormat());

    auto target = GetTextureTarget();
    auto faceTarget = GLDefs::GetTextureCubeFaceTarget(face);

    auto xoffset = static_cast<GLint>(region.x());
    auto yoffset = static_cast<GLint>(region.y());
    auto width = static_cast<GLsizei>(region.z());
    auto height = static_cast<GLsizei>(region.w());

    glBindTexture(target, mHandle);
    BRK_GL_CATCH_ERR();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    BRK_GL_CATCH_ERR();

    glTexSubImage2D(faceTarget, mipLevel, xoffset, yoffset, width, height, dataFormat, dataType, pixels);
    BRK_GL_CATCH_ERR();

    glBindTexture(target, 0);
    BRK_GL_CATCH_ERR();
}

void GLTexture::GenerateMipMaps() {
    assert(GetTextureType() == RHITextureType::Texture2d || GetTextureType() == RHITextureType::Texture2dArray || GetTextureType() == RHITextureType::TextureCube);
    assert(GetMipsCount() == ImageUtil::GetMaxMipsCount(GetWidth(), GetHeight(), GetDepth()));

    auto target = GetTextureTarget();

    glBindTexture(target, mHandle);
    BRK_GL_CATCH_ERR();

    glGenerateMipmap(target);
    BRK_GL_CATCH_ERR();

    glBindTexture(target, 0);
    BRK_GL_CATCH_ERR();
}

void GLTexture::Bind(uint32 location, uint32 slot) const {
    auto target = GetTextureTarget();

    glUniform1i(static_cast<GLint>(location), static_cast<GLint>(slot));
    BRK_GL_CATCH_ERR();

    glActiveTexture(GL_TEXTURE0 + slot);
    BRK_GL_CATCH_ERR();

    glBindTexture(target, mHandle);
    BRK_GL_CATCH_ERR();
}

GLenum GLTexture::GetTextureTarget() const {
    switch (GetTextureType()) {
        case RHITextureType::Texture2d:
            return GL_TEXTURE_2D;
        case RHITextureType::Texture2dArray:
            return GL_TEXTURE_2D_ARRAY;
        case RHITextureType::Texture3d:
            return GL_TEXTURE_3D;
        case RHITextureType::TextureCube:
            return GL_TEXTURE_CUBE_MAP;
        default:
            BRK_ERROR("Unknown texture type");
            return GL_NONE;
    }
}

BRK_NS_END