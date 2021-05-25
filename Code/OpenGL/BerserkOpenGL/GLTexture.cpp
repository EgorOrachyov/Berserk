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

#include <BerserkOpenGL/GLTexture.hpp>
#include <BerserkOpenGL/GLDefs.hpp>
#include <BerserkCore/Image/PixelUtil.hpp>
#include <BerserkCore/Platform/ThreadManager.hpp>

namespace Berserk {
    namespace RHI {

        GLTexture::GLTexture(const Texture::Desc &desc) {
            mDesc = desc;
        }

        GLTexture::~GLTexture() {
            if (mHandle) {
                glDeleteTextures(1, &mHandle);
                mHandle = 0;

                BERSERK_GL_LOG_INFO("Release texture: thread=\"{0}\"", ThreadManager::GetCurrentThread()->GetName());
            }
        }

        void GLTexture::Initialize() {
            switch (mDesc.textureType) {
                case TextureType::Texture2d:
                    Initialize2d();
                    break;
                case TextureType::Texture2dArray:
                    Initialize2dArray();
                    break;
                case TextureType::TextureCube:
                    InitializeCube();
                    break;
                default:
                    BERSERK_GL_LOG_ERROR("Unsupported texture type");
                    break;
            }
        }

        void GLTexture::Initialize2d() {
            assert(GetWidth() > 0);
            assert(GetHeight() > 0);
            assert(GetDepth() == 1);

            auto target = GetTextureTarget();
            auto internalFormat = GLDefs::GetTextureInternalFormat(GetTextureFormat());
            auto mipsCount = GetMipsCount();
            auto maxMips = PixelUtil::GetMaxMipsCount(GetWidth(), GetHeight(), 1);

            assert(mipsCount <= maxMips);

            glGenTextures(1, &mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glBindTexture(target, mHandle);
            BERSERK_GL_CATCH_ERRORS();

            auto width = GetWidth();
            auto height = GetHeight();

            auto dataFormat = GLDefs::GetTextureDataBaseFormat(GetTextureFormat());
            auto dataType = GLDefs::GetTextureDataType(GetTextureFormat());

            for (GLuint level = 0; level < mipsCount; level++) {
                glTexImage2D(target, level, internalFormat, width, height, 0, dataFormat, dataType,nullptr);
                BERSERK_GL_CATCH_ERRORS();

                if (width > 1) width /= 2;
                if (height > 1) height /= 2;
            }

            glBindTexture(target, 0);
            BERSERK_GL_CATCH_ERRORS();

            BERSERK_GL_LOG_INFO("Init texture 2d: thread=\"{0}\"", ThreadManager::GetCurrentThread()->GetName());
        }

        void GLTexture::Initialize2dArray() {
            assert(GetWidth() > 0);
            assert(GetHeight() > 0);
            assert(GetDepth() == 1);
            assert(GetArraySlices() > 0);

            auto target = GetTextureTarget();
            auto internalFormat = GLDefs::GetTextureInternalFormat(GetTextureFormat());
            auto mipsCount = GetMipsCount();
            auto maxMips = PixelUtil::GetMaxMipsCount(GetWidth(), GetHeight(), 1);

            assert(mipsCount <= maxMips);

            glGenTextures(1, &mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glBindTexture(target, mHandle);
            BERSERK_GL_CATCH_ERRORS();

            auto width = GetWidth();
            auto height = GetHeight();
            auto arraySize = GetArraySlices();

            auto dataFormat = GLDefs::GetTextureDataBaseFormat(GetTextureFormat());
            auto dataType = GLDefs::GetTextureDataType(GetTextureFormat());

            for (GLuint level = 0; level < mipsCount; level++) {
                glTexImage3D(target, level, internalFormat, width, height, arraySize, 0, dataFormat, dataType, nullptr);
                BERSERK_GL_CATCH_ERRORS();

                if (width > 1) width /= 2;
                if (height > 1) height /= 2;
            }

            glBindTexture(target, 0);
            BERSERK_GL_CATCH_ERRORS();

            BERSERK_GL_LOG_INFO("Init texture 2d array: thread=\"{0}\"", ThreadManager::GetCurrentThread()->GetName());
        }

        void GLTexture::InitializeCube() {
            assert(GetWidth() > 0);
            assert(GetHeight() > 0);
            assert(GetDepth() == 1);

            auto target = GetTextureTarget();
            auto internalFormat = GLDefs::GetTextureInternalFormat(GetTextureFormat());
            auto mipsCount = GetMipsCount();
            auto maxMips = PixelUtil::GetMaxMipsCount(GetWidth(), GetHeight(), 1);

            assert(mipsCount <= maxMips);

            glGenTextures(1, &mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glBindTexture(target, mHandle);
            BERSERK_GL_CATCH_ERRORS();

            auto width = GetWidth();
            auto height = GetHeight();

            auto dataFormat = GLDefs::GetTextureDataBaseFormat(GetTextureFormat());
            auto dataType = GLDefs::GetTextureDataType(GetTextureFormat());

            static const TextureCubemapFace faces[Limits::MAX_TEXTURE_CUBE_FACES] = {
                TextureCubemapFace::PositiveX,
                TextureCubemapFace::NegativeX,
                TextureCubemapFace::PositiveY,
                TextureCubemapFace::NegativeY,
                TextureCubemapFace::PositiveZ,
                TextureCubemapFace::NegativeZ
            };

            for (auto face: faces) {
                auto faceTarget = GetTextureCubeFaceTarget(face);

                auto w = width;
                auto h = height;

                for (GLuint level = 0; level < mipsCount; level++) {
                    glTexImage2D(faceTarget, level, internalFormat, w, h, 0, dataFormat, dataType, nullptr);
                    BERSERK_GL_CATCH_ERRORS();

                    if (w > 1) w /= 2;
                    if (h > 1) h /= 2;
                }
            }

            glBindTexture(target, 0);
            BERSERK_GL_CATCH_ERRORS();

            BERSERK_GL_LOG_INFO("Init texture cube: thread=\"{0}\"", ThreadManager::GetCurrentThread()->GetName());
        }

        void GLTexture::UpdateTexture2D(uint32 mipLevel, const Math::Rect2u &region, const PixelData& memory) {
            assert(TextureType::Texture2d == GetTextureType());
            assert(mipLevel < GetMipsCount());
            assert(region.x() <= region.z());
            assert(region.y() <= region.w());

            auto mipSize = PixelUtil::GetMipSize(mipLevel, GetWidth(), GetHeight());
            assert(region.z() <= mipSize.x());
            assert(region.w() <= mipSize.y());

            const auto* pixels = memory.GetData();
            auto dataFormat = GLDefs::GetPixelDataFormat(memory.GetDataFormat());
            auto dataType = GLDefs::GetPixelDataType(memory.GetDataType());
            auto target = GetTextureTarget();

            GLint xoffset = region.x();
            GLint yoffset = region.y();
            GLsizei width = region.z();
            GLsizei height = region.w();

            glBindTexture(target, mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            BERSERK_GL_CATCH_ERRORS();

            glTexSubImage2D(target, mipLevel, xoffset, yoffset, width, height, dataFormat, dataType, pixels);
            BERSERK_GL_CATCH_ERRORS();

            glBindTexture(target, 0);
            BERSERK_GL_CATCH_ERRORS();
        }

        void GLTexture::UpdateTexture2DArray(uint32 arrayIndex, uint32 mipLevel, const Math::Rect2u &region, const PixelData& memory) {
            assert(TextureType::Texture2dArray == GetTextureType());
            assert(arrayIndex < GetArraySlices());
            assert(mipLevel < GetMipsCount());
            assert(region.x() <= region.z());
            assert(region.y() <= region.w());

            auto mipSize = PixelUtil::GetMipSize(mipLevel, GetWidth(), GetHeight());
            assert(region.z() <= mipSize.x());
            assert(region.w() <= mipSize.y());

            const auto* pixels = memory.GetData();
            auto dataFormat = GLDefs::GetPixelDataFormat(memory.GetDataFormat());
            auto dataType = GLDefs::GetPixelDataType(memory.GetDataType());
            auto target = GetTextureTarget();

            GLint xoffset = region.x();
            GLint yoffset = region.y();
            GLsizei width = region.z();
            GLsizei height = region.w();

            glBindTexture(target, mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            BERSERK_GL_CATCH_ERRORS();

            glTexSubImage3D(target, mipLevel, xoffset, yoffset, arrayIndex, width, height, 1, dataFormat, dataType, pixels);
            BERSERK_GL_CATCH_ERRORS();

            glBindTexture(target, 0);
            BERSERK_GL_CATCH_ERRORS();
        }

        void GLTexture::UpdateTextureCube(TextureCubemapFace face, uint32 mipLevel, const Math::Rect2u &region, const PixelData &memory) {
            assert(TextureType::TextureCube == GetTextureType());
            assert(mipLevel < GetMipsCount());
            assert(region.x() <= region.z());
            assert(region.y() <= region.w());

            auto mipSize = PixelUtil::GetMipSize(mipLevel, GetWidth(), GetHeight());
            assert(region.z() <= mipSize.x());
            assert(region.w() <= mipSize.y());

            const auto* pixels = memory.GetData();
            auto dataFormat = GLDefs::GetPixelDataFormat(memory.GetDataFormat());
            auto dataType = GLDefs::GetPixelDataType(memory.GetDataType());

            auto target = GetTextureTarget();
            auto faceTarget = GetTextureCubeFaceTarget(face);

            GLint xoffset = region.x();
            GLint yoffset = region.y();
            GLsizei width = region.z();
            GLsizei height = region.w();

            glBindTexture(target, mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            BERSERK_GL_CATCH_ERRORS();

            glTexSubImage2D(faceTarget, mipLevel, xoffset, yoffset, width, height, dataFormat, dataType, pixels);
            BERSERK_GL_CATCH_ERRORS();

            glBindTexture(target, 0);
            BERSERK_GL_CATCH_ERRORS();
        }

        void GLTexture::GenerateMipMaps() {
            assert(GetTextureType() == TextureType::Texture2d || GetTextureType() == TextureType::Texture2dArray || GetTextureType() == TextureType::TextureCube);
            assert(GetMipsCount() == PixelUtil::GetMaxMipsCount(GetWidth(), GetHeight(), GetDepth()));

            auto target = GetTextureTarget();

            glBindTexture(target, mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glGenerateMipmap(target);
            BERSERK_GL_CATCH_ERRORS();

            glBindTexture(target, 0);
            BERSERK_GL_CATCH_ERRORS();
        }

        void GLTexture::Bind(uint32 location, uint32 slot) const {
            auto target = GetTextureTarget();

            glUniform1i(location, slot);
            BERSERK_GL_CATCH_ERRORS();

            glActiveTexture(GL_TEXTURE0 + slot);
            BERSERK_GL_CATCH_ERRORS();

            glBindTexture(target, mHandle);
            BERSERK_GL_CATCH_ERRORS();
        }

        bool GLTexture::UsageSampling() const {
            return mDesc.textureUsage.Get(TextureUsage::Sampling);
        }

        GLenum GLTexture::GetTextureTarget() const {
            switch (GetTextureType()) {
                case TextureType::Texture2d:
                    return GL_TEXTURE_2D;
                case TextureType::Texture2dArray:
                    return GL_TEXTURE_2D_ARRAY;
                case TextureType::Texture3d:
                    return GL_TEXTURE_3D;
                case TextureType::TextureCube:
                    return GL_TEXTURE_CUBE_MAP;
                default:
                    BERSERK_GL_LOG_ERROR("Unknown texture type");
                    return GL_NONE;
            }
        }

        GLenum GLTexture::GetTextureCubeFaceTarget(TextureCubemapFace face) const {
            auto id = (uint32) face;

            assert(id < Limits::MAX_TEXTURE_CUBE_FACES);

            static const GLenum targets[] = {
                GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
            };


            return targets[id];
        }

    }
}