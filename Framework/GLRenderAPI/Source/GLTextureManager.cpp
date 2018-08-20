//
// Created by Egor Orachyov on 04.07.2018.
//

#include "Managers/GLTextureManager.h"

namespace Berserk
{

    GLTextureManager::GLTextureManager()
    {
        mTextureList.init();
        mImporter.init();
    }

    GLTextureManager::~GLTextureManager()
    {
        mImporter.destroy();
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

    Texture* GLTextureManager::loadTexture(const CStaticString &name, const CStaticString &path)
    {
        Texture* t = getTexture(name);
        if (t != nullptr)
        {
            t->addReference();
            PUSH("GLTextureManager: Load texture %s ref %u", t->getName().getChars(), t->getReferences());
            return t;
        }

        t = createTexture(name);
        CString file;
        file = path;
        file += name;

        UINT32 width;
        UINT32 height;
        GLInternalTextureFormat internalTextureFormat = GLInternalTextureFormat::GLTF_RGB8;
        GLImageFormat imageFormat;
        GLDataType dataType;
        GLMipmaps mipmaps = GLMipmaps::GLM_USE;
        void* data;

        mImporter.importTexture(file.getChars(), width, height, imageFormat, dataType, data);

        if (data == nullptr)
        {
            WARNING("GLTextureManager: Cannot load texture with name %s path %s", name.getChars(), path.getChars());
            return nullptr; // todo: return error texture
        }

        auto texture = dynamic_cast<GLTexture*>(t);
        texture->create(width, height, internalTextureFormat, imageFormat, dataType, data, mipmaps);
//      texture->setFiltering(GLFiltering::GLF_LINEAR, GLFiltering::GLF_LINEAR);
        texture->setFiltering(GLFiltering::GLF_NEAREST, GLFiltering::GLF_NEAREST);
        texture->setWrapping(GLWrapping::GLW_CLAMP_TO_EDGE, GLWrapping::GLW_CLAMP_TO_EDGE);

        if (!texture->isLoaded())
        {
            WARNING("GLTextureManager: Cannot load texture with name %s path %s", name.getChars(), path.getChars());
            return nullptr; // todo: return error texture
        }
        else
        {
            PUSH("GLTextureManager: Load texture %s ref %u", t->getName().getChars(), t->getReferences());
            return texture;
        }
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