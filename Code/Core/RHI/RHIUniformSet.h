/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIUNIFORMSET_H
#define BERSERK_RHIUNIFORMSET_H

#include <RHI/RHIResource.h>

namespace Berserk {

    class RHIUniformSet : public RHIResource {
    public:
        ~RHIUniformSet() override = default;

        /** @return Uniform textures for the set */
        const TArray<RHIUniformTextureDesc> &getTextures() const { return mTextures; }

        /** @return Uniform blocks for the set */
        const TArray<RHIUniformBlockDesc> &getUniformBlocks() const { return mUniformBlocks; }

    protected:
        /** Uniform textures for the set */
        TArray<RHIUniformTextureDesc> mTextures;

        /** Uniform blocks for the set */
        TArray<RHIUniformBlockDesc> mUniformBlocks;
    };

}

#endif //BERSERK_RHIUNIFORMSET_H
