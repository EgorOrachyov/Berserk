/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <GLTexture.h>

namespace Berserk {

    TArray<uint8> GLTexture::mCachedPixelDataTmpBuffer;
    TPtrShared<RHITexture> GLTexture::mDefaultWhiteTexture;
    TPtrShared<RHITexture> GLTexture::mDefaultBlackTexture;

}