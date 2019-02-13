//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_ITEXTURE_H
#define BERSERK_ITEXTURE_H

#include "Public/Misc/Types.h"
#include "Public/Platform/ISampler.h"

namespace Berserk
{

    class ITexture
    {
    public:

        enum TargetType : uint32
        {
            NORMAL_MAP,
            AMBIENT_MAP,
            DIFFUSE_MAP,
            SPECULAR_MAP,
            REFLECTION_MAP,
            DISPLACEMENT_MAP
        };

        enum TextureType : uint32
        {
            TEXTURE_1D,
            TEXTURE_2D,
            TEXTURE_3D
        };

        enum PixelFormat : uint32
        {
            FORMAT_R,
            FORMAT_RG,
            FORMAT_RGB,
            FORMAT_BGR,
            FORMAT_RGBA,
            FORMAT_ABGR,
            FORMAT_DEPTH,
            FORMAT_DEPTH_AND_STENCIL
        };

        enum PixelType : uint32
        {
            GL_INT,
            GL_BYTE,
            GL_SHORT,
            GL_FLOAT,
            GL_HALF_FLOAT,
            GL_UNSIGNED_INT,
            GL_UNSIGNED_BYTE,
            GL_UNSIGNED_SHORT
        };

        enum StorageFormat : uint32
        {
            RGB8,
            RGBA8,
            RGB32F,
            RGBA32F,
            DEPTH24,
            DEPTH24_STENCIL8
        };

    public:

        virtual ~ITexture() = 0;

        virtual void   create(uint32 width, uint32 height, StorageFormat storage) = 0;

        virtual void   create(uint32 width, uint32 height, StorageFormat storage, void* data, PixelFormat format, bool genMipMaps) = 0;

        virtual void   bind(ISampler* sampler, uint32 textureSlot) = 0;

        virtual void   getData(uint32 depth, uint32 size, PixelType destination, void* data) = 0;

        virtual uint32 getTargetType() = 0;

        virtual uint32 getPixelFormat() = 0;

        virtual uint32 getTextureType() = 0;

        virtual uint32 getStorageFormat() = 0;

        virtual bool   getMipMapsGen() = 0;

        virtual uint32 getHandle() = 0;

        virtual uint32 getWidth() = 0;

        virtual uint32 getHeight() = 0;

        virtual uint32 getGPUMemoryUsage() = 0;

    };

} // namespace Berserk

#endif //BERSERK_ITEXTURE_H