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
            assert(GetTextureType() == TextureType::Texture2d);

            switch (mDesc.textureType) {
                case TextureType::Texture2d:
                    Initialize2d();
                    break;
                default:
                    break;
            }
        }

        void GLTexture::Initialize2d() {
            assert(GetWidth() > 0);
            assert(GetHeight() > 0);
            assert(GetDepth() == 1);

            auto internalFormat = GLDefs::GetTextureInternalFormat(GetTextureFormat());
            auto mipsCount = GetMipsCount();
            auto maxMips = PixelUtil::GetMaxMipsCount(GetWidth(), GetHeight(), 1);

            assert(mipsCount <= maxMips);

            glGenTextures(1, &mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glBindTexture(GL_TEXTURE_2D, mHandle);
            BERSERK_GL_CATCH_ERRORS();

            auto width = GetWidth();
            auto height = GetHeight();

            auto dataFormat = GLDefs::GetTextureDataBaseFormat(GetTextureFormat());
            auto dataType = GLDefs::GetTextureDataType(GetTextureFormat());

            for (GLuint level = 0; level < mipsCount; level++) {
                glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, 0, dataFormat, dataType,nullptr);
                BERSERK_GL_CATCH_ERRORS();

                if (width > 1) width /= 2;
                if (height > 1) height /= 2;
            }

            glBindTexture(GL_TEXTURE_2D, 0);
            BERSERK_GL_CATCH_ERRORS();

            BERSERK_GL_LOG_INFO("Create texture 2d: thread=\"{0}\"", ThreadManager::GetCurrentThread()->GetName());
        }

        void GLTexture::UpdateTexture2D(uint32 mipLevel, const Math::Rect2u &region, const RefCounted<PixelData> &memory) {
            assert(mipLevel < GetMipsCount());
            assert(region.x() <= region.z());
            assert(region.y() <= region.w());
            assert(memory);

            auto mipSize = PixelUtil::GetMipSize(mipLevel, GetWidth(), GetHeight());
            assert(region.z() <= mipSize.z());
            assert(region.w() <= mipSize.w());

            const auto* pixels = memory->GetData();
            auto dataFormat = GLDefs::GetPixelDataFormat(memory->GetDataFormat());
            auto dataType = GLDefs::GetPixelDataType(memory->GetDataType());

            GLint xoffset = region.x();
            GLint yoffset = region.y();
            GLsizei width = region.z();
            GLsizei height = region.w();

            glBindTexture(GL_TEXTURE_2D, mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glTexSubImage2D(GL_TEXTURE_2D, mipLevel, xoffset, yoffset, width, height, dataFormat, dataType, pixels);
            BERSERK_GL_CATCH_ERRORS();

            glBindTexture(GL_TEXTURE_2D, 0);
            BERSERK_GL_CATCH_ERRORS();
        }

        void GLTexture::GenerateMipMaps() {
            assert(GetTextureType() == TextureType::Texture2d);
            assert(GetMipsCount() == PixelUtil::GetMaxMipsCount(GetWidth(), GetHeight(), GetDepth()));

            glBindTexture(GL_TEXTURE_2D, mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glGenerateMipmap(GL_TEXTURE_2D);
            BERSERK_GL_CATCH_ERRORS();

            glBindTexture(GL_TEXTURE_2D, 0);
            BERSERK_GL_CATCH_ERRORS();
        }

        void GLTexture::Bind(uint32 location, uint32 slot) const {
            glUniform1i(location, slot);
            BERSERK_GL_CATCH_ERRORS();

            glActiveTexture(GL_TEXTURE0 + slot);
            BERSERK_GL_CATCH_ERRORS();

            glBindTexture(GL_TEXTURE_2D, mHandle);
            BERSERK_GL_CATCH_ERRORS();
        }

    }
}