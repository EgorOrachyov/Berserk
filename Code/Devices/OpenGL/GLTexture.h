/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLTEXTURE_H
#define BERSERK_GLTEXTURE_H

#include <RHI/RHIResources.h>
#include <GLDefinitions.h>
#include <GLErrors.h>

namespace Berserk {

    class GLTexture : public RHITexture {
    public:

        ~GLTexture() override {
            destroy();
        }

        void create(uint32 width, uint32 height, EMemoryType memoryType, EPixelFormat pixelFormat, bool useMipMaps, const uint8 *data) {
            BERSERK_COND_ERROR_FAIL(width > 0, "Attempt to create texture of 0 size");
            BERSERK_COND_ERROR_FAIL(height > 0, "Attempt to create texture of 0 size");
            BERSERK_COND_ERROR_FAIL(pixelFormat != EPixelFormat::Undefined, "Attempt to create texture of undefined format");

            GLint internalFormat;
            GLenum format;
            GLenum type;
            GLDefinitions::getTexturePixelFormat(pixelFormat, internalFormat, format, type);

            glGenTextures(1, &mTextureHandle);
            glBindTexture(GL_TEXTURE_2D, mTextureHandle);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
            if (useMipMaps) glGenerateMipmap(GL_TEXTURE_2D);

            ///////////// Debug
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            ///////////// Debug

            glBindTexture(GL_TEXTURE_2D, 0);

            BERSERK_CATCH_OPENGL_ERRORS();

            mWidth = width;
            mHeight = height;
            mDepth = 1;
            mMemoryType = memoryType;
            mTextureType = ETextureType::Texture2D;
            mPixelFormat = pixelFormat;
        }

        void create(uint32 width, uint32 height, EMemoryType memoryType, EPixelFormat pixelFormat, bool useMipMaps) {
            create(width, height, memoryType, pixelFormat, useMipMaps, nullptr);
        }

        void create(bool useMipMaps, const Image& image) {
            // Prior any operations we must copy image and flip it along Y axis,
            // since OpenGL expects that 0.0y is a bottom left corner of the image.
            // Current image class convention says, that 0.0y is an upper left corner.

            auto width = image.getWidth();
            auto height = image.getHeight();
            auto pixelSize = image.getPixelSize();
            auto pixelFormat = image.getPixelFormat();

            auto size = image.getPixelData().size();
            mCachedPixelDataTmpBuffer.clearNoDestructorCall();
            mCachedPixelDataTmpBuffer = image.getPixelData();
            Image::pixelDataFlipAlongY(width, height, pixelSize, mCachedPixelDataTmpBuffer);

            const auto* data = mCachedPixelDataTmpBuffer.data();

            create(width, height, EMemoryType::Dynamic, pixelFormat, useMipMaps, data);
        }

        void destroy() {
            glDeleteTextures(1, &mTextureHandle);
            mTextureHandle = 0;

            BERSERK_CATCH_OPENGL_ERRORS();
        }

        GLuint getTextureHandle() {
            return mTextureHandle;
        }

        // todo: Debug, remove this
        void bind(uint32 slot) {
            glBindSampler(slot, 0);
            glUniform1i(0, slot);
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, mTextureHandle);
        }

        static void createDefaultTextures() {
            Image whiteImage;
            whiteImage.create(1, 1, EPixelFormat::R8G8B8A8, Color4f(1.0f));
            auto whiteTexture = TPtrShared<GLTexture>::make();
            whiteTexture->create(false, whiteImage);
            mDefaultWhiteTexture = (TPtrShared<RHITexture>) whiteTexture;

            Image blackImage;
            blackImage.create(1, 1, EPixelFormat::R8G8B8A8, Color4f(0.0f));
            auto blackTexture = TPtrShared<GLTexture>::make();
            blackTexture->create(false, blackImage);
            mDefaultBlackTexture = (TPtrShared<RHITexture>) blackTexture;
        }

        static void releaseDefaultTextures() {
            mDefaultWhiteTexture.free();
            mDefaultBlackTexture.free();
        }

        static TPtrShared<RHITexture> &getDefaultWhiteTexture() {
            return mDefaultWhiteTexture;
        }

        static TPtrShared<RHITexture> &getDefaultBlackTexture() {
            return mDefaultBlackTexture;
        }

    private:

        GLuint mTextureHandle;

        /** Default texture (for silent substitution of lost/invalid textures) */
        static TPtrShared<RHITexture> mDefaultWhiteTexture;
        /** Default texture (for silent substitution of lost/invalid textures) */
        static TPtrShared<RHITexture> mDefaultBlackTexture;
        /** Static cached pixel data buffer for fast textures flipping */
        static TArray<uint8> mCachedPixelDataTmpBuffer;

    };

}

#endif //BERSERK_GLTEXTURE_H
