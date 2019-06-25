//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_GLTEXTURE_H
#define BERSERK_GLTEXTURE_H

#include "Platform/ITexture.h"
#include "Platform/GLSampler.h"
#include "Strings/Old/String.h"

namespace Berserk
{

    namespace Resources
    {

        /**
         * OpengGL implementation for texture class
         */
        class GRAPHICS_API GLTexture : public ITexture
        {
        public:

            ~GLTexture();

            /** @copydoc IResource::initialize() */
            void initialize(const char *name) override;

            /** @copydoc IResource::addReference() */
            void addReference() override;

            /** @copydoc IResource::release() */
            void release() override;

            /** @copydoc IResource::getReferenceCount() */
            uint32 getReferenceCount() override;

            /** @copydoc IResource::getMemoryUsage() */
            uint32 getMemoryUsage() override;

            /** @copydoc IResource::getName() */
            const char *getName() override;

        public:

            /** @copydoc ITexture::create(width,height,storageFormat) */
            void create(uint32 width, uint32 height, IRenderDriver::StorageFormat storageFormat) override;

            /** @copydoc ITexture::create(size,storageFormat,pixelFormat) */
            void create(uint32 size, IRenderDriver::StorageFormat storageFormat) override;

            /** @copydoc ITexture::create(width,height,storageFormat,pixelFormat,pixelType,data,genMipMaps) */
            void create(uint32 width,
                        uint32 height,
                        IRenderDriver::StorageFormat storageFormat,
                        IRenderDriver::PixelFormat pixelFormat,
                        IRenderDriver::DataType pixelType,
                        void *data,
                        bool genMipMaps) override;

            /** @copydoc ITexture::bind(textureSlot) */
            void bind(uint32 textureSlot) override;

            /** @copydoc ITexture::getData() */
            void getData(uint32 depth,
                         IRenderDriver::PixelFormat format,
                         IRenderDriver::DataType type,
                         void *data) override;

            /** @copydoc ITexture::getSize() */
            void getSize(uint32 &width, uint32 &height) override;

            /** @copydoc ITexture::setFiltering() */
            void setFiltering(IRenderDriver::SamplerFilter min, IRenderDriver::SamplerFilter mag) override;

            /** @copydoc ITexture::setWrapping() */
            void setWrapping(IRenderDriver::SamplerWrapMode wrap) override;

            /** @copydoc ITexture::setBorderColor() */
            void setBorderColor(const Vec4f &color) override;

            /** @copydoc ITexture::getMipMapsGen() */
            bool getMipMapsGen() override;

            /** @copydoc ITexture::getSampler() */
            ISampler *getSampler() override;

            /** @copydoc ITexture::getHandle() */
            uint32 getHandle() override;

            /** @copydoc ITexture::getWidth() */
            uint32 getWidth() override;

            /** @copydoc ITexture::getHeight() */
            uint32 getHeight() override;

            /** @copydoc ITexture::getGPUMemoryUsage() */
            uint32 getGPUMemoryUsage() override;

        private:

            friend class GLFrameBuffer;
            friend class GLDepthBuffer;
            friend class GLTextureManager;

            uint32 mPixelFormat;
            uint32 mTextureType;
            uint32 mStorageFormat;
            bool mGenMipMaps;

            uint32 mWidth;
            uint32 mHeight;

            uint32 mTextureID;
            uint32 mReferenceCount;

            ISampler *mSampler;
            CString mResourceName;
        };

    } // namespace Resources

} // namespace Berserk

#endif //BERSERK_GLTEXTURE_H