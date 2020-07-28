/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHITEXTURE_H
#define BERSERK_RHITEXTURE_H

#include <RHI/RHIResource.h>

namespace Berserk {

    class RHITexture : public RHIResource {
    public:
        ~RHITexture() override = default;

        /** @return Texture width in pixels */
        uint32 getWidth() const { return mWidth; }

        /** @return Texture height in pixels */
        uint32 getHeight() const { return mHeight; }

        /** @return Texture depth in pixels */
        uint32 getDepth() const { return mDepth; }

        /** @return Texture RHI type */
        ETextureType getTextureType() { return mTextureType; }

        /** @return Texture memory type (host visibility) */
        EBufferUsage getMemoryType() { return mMemoryType; }

        /** @return Texture data format per pixels */
        EPixelFormat getPixelFormat() const { return mPixelFormat; }

    protected:
        /** Texture width in pixels */
        uint32 mWidth = 0;

        /** Texture height in pixels */
        uint32 mHeight = 0;

        /** Texture depth in pixels */
        uint32 mDepth = 1;

        /** Texture RHI type */
        ETextureType mTextureType;

        /** Texture memory type (host visibility) */
        EBufferUsage mMemoryType;

        /** Texture data format per pixels */
        EPixelFormat mPixelFormat;
    };

}

#endif //BERSERK_RHITEXTURE_H
