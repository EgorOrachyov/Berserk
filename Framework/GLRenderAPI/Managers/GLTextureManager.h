//
// Created by Egor Orachyov on 04.07.2018.
//

#ifndef BERSERKENGINE_GLTEXTUREMANAGER_H
#define BERSERKENGINE_GLTEXTUREMANAGER_H

#include "Render/TextureManager.h"
#include "Containers/LinkedList.h"
#include "Texture/GLTexture.h"
#include "Texture/GLTextureImporter.h"

namespace Berserk
{

    class GLTextureManager : public TextureManager
    {
    public:

        GLTextureManager();
        virtual ~GLTextureManager();

        UINT32 getMemoryUsage() const override;

        Texture* getTexture(UINT32 id) override;
        Texture* getTexture(const CStaticString& name) override;

        Texture* createTexture(const CStaticString& name) override;
        Texture* renameTexture(const CStaticString& oldName, const CStaticString& newName) override;
        Texture* loadTexture(const CStaticString& name, const CStaticString& path) override;

        bool deleteTexture(UINT32 id) override;
        bool deleteTexture(const CStaticString& name) override;
        bool deleteTexture(Texture* toDelete) override;

    private:

        GLTextureImporter mImporter;
        LinkedList<GLTexture> mTextureList;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLTEXTUREMANAGER_H