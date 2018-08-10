//
// Created by Egor Orachyov on 04.07.2018.
//

#include "Managers/GLTextureManager.h"

namespace Berserk
{

    GLTextureManager::GLTextureManager()
    {
        mTextureList.init();
    }

    UINT32 GLTextureManager::getMemoryUsage() const
    {
        UINT32 memUsage = 0;

        if (mTextureList.getSize())
        {
            memUsage += mTextureList.getSize() * mTextureList.getFirst().getMemoryUsage();
        }

        return memUsage;
    }

    Texture* GLTextureManager::getTexture(UINT32 id)
    {
        return &mTextureList.get(id);
    }

    Texture* GLTextureManager::getTexture(const CStaticString& name)
    {
        mTextureList.iterate(true);
        while (mTextureList.iterate())
        {
            if (mTextureList.getCurrent().getName() == name)
            {
                return &mTextureList.getCurrent();
            }
        }

        return nullptr;
    }

    Texture* GLTextureManager::createTexture(const CStaticString &name)
    {
        Texture* found = getTexture(name);
        if (found != nullptr)
        {
            found->addReference();
            return found;
        }

        GLTexture texture;
        texture.setName(name);
        texture.addReference();
        mTextureList.add(texture);
        return &mTextureList.getLast();
    }

    Texture* GLTextureManager::renameTexture(const CStaticString& oldName, const CStaticString& newName)
    {
        mTextureList.iterate(true);
        while (mTextureList.iterate())
        {
            if (mTextureList.getCurrent().getName() == oldName)
            {
                GLTexture* texture = &mTextureList.getCurrent();
                texture->setName(newName);
                return texture;
            }
        }

        return nullptr;
    }

    bool GLTextureManager::deleteTexture(UINT32 id)
    {
        GLTexture* texture = &mTextureList.get(id);

        if (texture == nullptr)
        {
            return false;
        }

        texture->release();
        if (texture->getReferences() == 0)
        {
            mTextureList.remove(*texture);
        }

        return true;
    }

    bool GLTextureManager::deleteTexture(const CStaticString& name)
    {
        GLTexture* texture = nullptr;

        mTextureList.iterate(true);
        while (mTextureList.iterate())
        {
            if (mTextureList.getCurrent().getName() == name)
            {
                texture = &mTextureList.getCurrent();
                break;
            }
        }

        if (texture == nullptr)
        {
            return false;
        }

        texture->release();
        if (texture->getReferences() == 0)
        {
            mTextureList.remove(*texture);
        }

        return true;
    }


    bool GLTextureManager::deleteTexture(Texture* toDelete)
    {
        GLTexture* texture = nullptr;

        mTextureList.iterate(true);
        while (mTextureList.iterate())
        {
            if (mTextureList.getCurrent().getName() == toDelete->getName())
            {
                texture = &mTextureList.getCurrent();
                break;
            }
        }

        if (texture == nullptr)
        {
            return false;
        }

        texture->release();
        if (texture->getReferences() == 0)
        {
            mTextureList.remove(*texture);
        }

        return true;
    }
    
} // namespace Berserk