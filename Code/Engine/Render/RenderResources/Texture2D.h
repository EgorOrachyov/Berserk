/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TEXTURE2D_H
#define BERSERK_TEXTURE2D_H

#include <RenderResources/Texture.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief 2D Texture
         *
         * Represents classical 2D texture create from image or another data.
         * Possibly can be used as a render target for color/depth rendering.
         */
        class Texture2D : public Texture {
        public:

            Texture2D(const CString &textureName, const Image &image, bool mipMaps = true);
            ~Texture2D() override = default;

            bool isInitialized() const override;
            bool isInitializedRHI() const override;

            Size2i getSize() const { return Size2i(mTextureRHI->getWidth(), mTextureRHI->getHeight()); };

        private:

        };

    }
}




#endif //BERSERK_TEXTURE2D_H
