//
// Created by Egor Orachyov on 08.08.2018.
//

#ifndef BERSERKENGINE_TEXTUREMANAGER_H
#define BERSERKENGINE_TEXTUREMANAGER_H

#include "Texture.h"

namespace Berserk
{
    class TextureManager
    {
    public:

        virtual ~TextureManager() = default;

        virtual UINT32 getMemoryUsage() const = 0;

        virtual Texture* getTexture(UINT32 id) = 0;
        virtual Texture* getTexture(const CStaticString& name) = 0;

        virtual Texture* createTexture(const CStaticString& name) = 0;
        virtual Texture* renameTexture(const CStaticString& oldName, const CStaticString& newName) = 0;

        virtual bool deleteTexture(UINT32 id) = 0;
        virtual bool deleteTexture(const CStaticString& name) = 0;
        virtual bool deleteTexture(Texture* toDelete) = 0;
    };
}

#endif //BERSERKENGINE_TEXTUREMANAGER_H