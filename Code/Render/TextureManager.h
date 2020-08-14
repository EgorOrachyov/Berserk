/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TEXTUREMANAGER_H
#define BERSERK_TEXTUREMANAGER_H

#include <RenderResources/Texture2D.h>

namespace Berserk {
    namespace Render {

        class TextureManager {
        public:
            TextureManager();
            ~TextureManager();

            /** @return Default dummy white texture */
            const TPtrShared<Texture2D> &getWhiteTexture() const { return mWhiteTexture; }

            /** @return Default dummy black texture */
            const TPtrShared<Texture2D> &getBlackTexture() const { return mBlackTexture; }

            /** @return Singleton instance */
            static TextureManager& getSingleton();

        private:
            TPtrShared<Texture2D> mWhiteTexture;
            TPtrShared<Texture2D> mBlackTexture;

            static TextureManager* gTextureManager;
        };

    }
}




#endif //BERSERK_TEXTUREMANAGER_H
