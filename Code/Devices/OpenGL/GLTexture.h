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

        bool create2d(uint32 width, uint32 height, EBufferUsage bufferUsage, EPixelFormat pixelFormat, bool useMipMaps, const uint8 *data) {
            BERSERK_COND_ERROR_RET_VALUE(false, width > 0, "Attempt to create texture of 0 size");
            BERSERK_COND_ERROR_RET_VALUE(false, height > 0, "Attempt to create texture of 0 size");
            BERSERK_COND_ERROR_RET_VALUE(false, pixelFormat != EPixelFormat::Unknown, "Attempt to create texture of undefined format");

            GLint internalFormat;
            GLenum format;
            GLenum type;
            GLDefinitions::getTexturePixelFormat(pixelFormat, internalFormat, format, type);

            glGenTextures(1, &mTextureHandle);
            glBindTexture(GL_TEXTURE_2D, mTextureHandle);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
            if (useMipMaps) glGenerateMipmap(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, 0);

            BERSERK_CATCH_OPENGL_ERRORS();

            mWidth = width;
            mHeight = height;
            mDepth = 1;
            mBufferUsage = bufferUsage;
            mTextureType = ETextureType::Texture2D;
            mPixelFormat = pixelFormat;

            return true;
        }

        bool create2d(uint32 width, uint32 height, EBufferUsage bufferUsage, EPixelFormat pixelFormat, bool useMipMaps) {
            return create2d(width, height, bufferUsage, pixelFormat, useMipMaps, nullptr);
        }

        bool create2d(EBufferUsage bufferUsage, bool useMipMaps, const Image &image) {
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

            return create2d(width, height, bufferUsage, pixelFormat, useMipMaps, data);
        }

        void destroy() {
            if (mTextureHandle) {
                glDeleteTextures(1, &mTextureHandle);
                mTextureHandle = 0;

                BERSERK_CATCH_OPENGL_ERRORS();
            }
        }

        GLenum getTextureTypeGL() {
            return mType;
        }

        GLuint getTextureHandle() {
            return mTextureHandle;
        }

        void bind(uint32 location, uint32 slot) {
            glUniform1i(location, slot);
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, mTextureHandle);
        }

        static void createDefaultTextures() {
            Image whiteImage;
            whiteImage.create(1, 1, EPixelFormat::R8G8B8A8, Color4f(1.0f));
            auto whiteTexture = TPtrShared<GLTexture>::make();
            whiteTexture->create2d(EBufferUsage::Static, false, whiteImage);
            mDefaultWhiteTexture = (TPtrShared<RHITexture>) whiteTexture;

            Image blackImage;
            blackImage.create(1, 1, EPixelFormat::R8G8B8A8, Color4f(0.0f));
            auto blackTexture = TPtrShared<GLTexture>::make();
            blackTexture->create2d(EBufferUsage::Static, false, blackImage);
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

        GLenum mType;
        GLuint mTextureHandle = 0;

        /** Default texture (for silent substitution of lost/invalid textures) */
        static TPtrShared<RHITexture> mDefaultWhiteTexture;
        /** Default texture (for silent substitution of lost/invalid textures) */
        static TPtrShared<RHITexture> mDefaultBlackTexture;
        /** Static cached pixel data buffer for fast textures flipping */
        static TArray<uint8> mCachedPixelDataTmpBuffer;

    };

}

#endif //BERSERK_GLTEXTURE_H
