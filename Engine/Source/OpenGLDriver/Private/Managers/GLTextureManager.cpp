//
// Created by Egor Orachyov on 01.03.2019.
//

#include "Managers/GLTextureManager.h"
#include "Platform/GLProfile.h"

namespace Berserk::Resources
{

    GLTextureManager::GLTextureManager(Importers::IImageImporter* importer, const char *path) : mTextures(INITIAL_TEXTURES_COUNT),
                                                                                                mSamplers(INITIAL_SAMPLERS_COUNT),
                                                                                                mTexturesPath(path),
                                                                                                mImageImporter(importer)
    {
        ASSERT(importer, "Nullptr image importer");

        {
            mSamplerLinear = createSampler("LinearFilteringClampEdge");
            mSamplerNearest = createSampler("NearestFilteringClampEdge");

            mSamplerLinear->create(IRenderDriver::FILTER_LINEAR,
                                   IRenderDriver::FILTER_LINEAR,
                                   IRenderDriver::WRAP_CLAMP_TO_EDGE);

            mSamplerNearest->create(IRenderDriver::FILTER_NEAREST,
                                    IRenderDriver::FILTER_NEAREST,
                                    IRenderDriver::WRAP_CLAMP_TO_EDGE);
        }

        {
            mDefaultTexture = loadTexture("{TEXTURES}/", "Default/DefaultTexture.png");
            mDefaultHelperTexture = loadTexture("{TEXTURES}/", "Default/DefaultHelperTexture.png");
        }

        PUSH("GLTextureManager: initialize");
    }

    GLTextureManager::~GLTextureManager()
    {
        {
            // Explicit deletions for textures in the manager,
            // Which references were not decreased to the 0
            // (Avoids memory leaks)

            for (auto current = mTextures.iterate(); current != nullptr; current = mTextures.next())
            {
#if PROFILE_GL_TEXTURE_MANAGER
                PUSH("GLTextureManager: release texture [name: '%s']", current->getName());
#endif

                current->mReferenceCount = 0;
                current->release();
            }
        }

        {
            // Explicit deletions for samplers in the manager,
            // Which references were not decreased to the 0
            // (Avoids memory leaks)

            for (auto current = mSamplers.iterate(); current != nullptr; current = mSamplers.next())
            {
#if PROFILE_GL_TEXTURE_MANAGER
                PUSH("GLTextureManager: release sampler [name: '%s']", current->getName());
#endif

                current->mReferenceCount = 0;
                current->release();
            }
        }

        PUSH("GLTextureManager: de-initialize");
    }

    void GLTextureManager::renameTexture(ITexture *texture, const char *name)
    {
#if PROFILE_GL_TEXTURE_MANAGER
        PUSH("GLTextureManager: rename texture [old name: '%s'][new name: '%s']", texture->getName(), name);
#endif

        auto renamed = dynamic_cast<GLTexture*>(texture);
        renamed->mResourceName = name;
    }

    void GLTextureManager::renameSampler(ISampler *sampler, const char *name)
    {
#if PROFILE_GL_TEXTURE_MANAGER
        PUSH("GLTextureManager: rename sampler [old name: '%s'][new name: '%s']", sampler->getName(), name);
#endif

        auto renamed = dynamic_cast<GLSampler*>(sampler);
        renamed->mResourceName = name;
    }

    void GLTextureManager::saveTexture(ITexture *texture, const char *path)
    {
        // todo
    }

    void GLTextureManager::bindSampler(ITexture *texture, ISampler *sampler)
    {
        auto target = dynamic_cast<GLTexture*>(texture);

        if (target->mSampler == nullptr)
        {
            sampler->addReference();
            target->mSampler = sampler;
        }
        else
        {
            auto old = target->mSampler;
            sampler->addReference();
            target->mSampler = sampler;
            deleteSampler(old);
        }
    }

    void GLTextureManager::deleteTexture(ITexture *texture)
    {
        CText name(texture->getName());
        texture->release();

        if (texture->getReferenceCount() == 0)
        {
            auto sampler = texture->getSampler();

            if (sampler)
            {
                deleteSampler(sampler);
            }

#if PROFILE_GL_TEXTURE_MANAGER
            PUSH("GLTextureManager: delete texture [name: '%s']", name.get());
#endif
            mTextures.remove((GLTexture*)texture);
        }
    }

    void GLTextureManager::deleteSampler(ISampler *sampler)
    {
        CText name(sampler->getName());
        sampler->release();

        if (sampler->getReferenceCount() == 0)
        {
#if PROFILE_GL_TEXTURE_MANAGER
            PUSH("GLTextureManager: delete sampler [name: '%s']", name.get());
#endif
            mSamplers.remove((GLSampler*)sampler);
        }
    }

    ITexture* GLTextureManager::createTexture(const char *name)
    {
        ITexture* found = findTexture(name);

        if (found)
        {
            WARNING("GLTextureManager: texture already exist [name: '%s']", name);
            return nullptr;
        }

        {
            GLTexture texture;
            texture.initialize(name);
            texture.addReference();

            mTextures += texture;

#if PROFILE_GL_TEXTURE_MANAGER
            PUSH("GLTextureManager: create texture [name: '%s'][ref: %u]", texture.getName(), texture.getReferenceCount());
#endif
        }

        return mTextures.getLast();
    }

    ITexture* GLTextureManager::findTexture(const char *name)
    {
        for (auto current = mTextures.iterate(); current != nullptr; current = mTextures.next())
        {
            if (current->mResourceName == name)
            {
                return current;
            }
        }

        return nullptr;
    }

    ITexture* GLTextureManager::getTexture(const char *name)
    {
        ITexture* found = findTexture(name);

        if (found)
        {
            found->addReference();
#if PROFILE_GL_TEXTURE_MANAGER
            PUSH("GLTextureManager: find texture [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
#endif
            return found;
        }

        return nullptr;
    }

    ITexture * GLTextureManager::loadTexture(const char *path, const char *name)
    {
        ITexture* found = findTexture(name);

        if (found)
        {
            found->addReference();
#if PROFILE_GL_TEXTURE_MANAGER
            PUSH("GLTextureManager: load texture [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
#endif
            return found;
        }

        {
            CPath filename(path);
            filename = filename.replace(CPath("{TEXTURES}"), CPath(mTexturesPath.get()));
            filename += name;

            Importers::IImageImporter::ImageData data;

            auto loaded = mImageImporter->import(filename.get(), data);

            if (!loaded)
            {
                WARNING("GLTextureManager: failed to load texture [name: '%s']", filename.get());
                return getDefaultHelperTexture();
            }

            auto texture = new(mTextures.preallocate()) GLTexture;
            texture->initialize(name);
            texture->addReference();
            texture->create(data.width, data.height, data.storageFormat, data.pixelFormat, data.pixelType, data.buffer, true);
            texture->mSampler = getSamplerLinear();

#if PROFILE_GL_TEXTURE_MANAGER
            PUSH("GLTextureManager: load texture [name: '%s'][ref: %u]", texture->getName(), texture->getReferenceCount());
#endif

            return texture;
        }

    }

    ITexture* GLTextureManager::loadTextureFromXML(const char *name, XMLNode &node)
    {
        if (name)
        {
            ITexture* found = findTexture(name);

            if (found)
            {
                found->addReference();
#if PROFILE_GL_TEXTURE_MANAGER
                PUSH("GLTextureManager: find texture [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
#endif
                return found;
            }
        }

        const char* path = node.getAttribute("path").getValue();
        const char* resourcename = path;

        int32 offset = Strings<char, '\0'>::strstr(resourcename, "}/");
        if (offset >= 0)
        {
            resourcename += offset + 2;
        }
        else
        {
            offset = Strings<char, '\0'>::strstr(resourcename, "../");
            if (offset >= 0) resourcename += offset + 3;
        }

        /*

        CPath filename(node.getAttribute("path").getValue());
        filename = filename.replace(CPath("{TEXTURES}"), CPath(mTexturesPath.get()));

        printf("name: '%s' path: '%s' \n", resourcename, filename.get());

        */

        return loadTexture(path, resourcename);
    }

    ITexture* GLTextureManager::copyTexture(ITexture *texture)
    {
        texture->addReference();
        return texture;
    }

    ITexture* GLTextureManager::getDefaultTexture()
    {
        mDefaultTexture->addReference();
        return mDefaultTexture;
    }

    ITexture* GLTextureManager::getDefaultHelperTexture()
    {
        mDefaultHelperTexture->addReference();
        return mDefaultHelperTexture;
    }

    ISampler* GLTextureManager::createSampler(const char *name)
    {
        ISampler* found = findSampler(name);

        if (found)
        {
            WARNING("GLTextureManager: sampler already exist [name: '%s']", name);
            return nullptr;
        }

        {
            GLSampler sampler;
            sampler.initialize(name);
            sampler.addReference();

            mSamplers += sampler;

#if PROFILE_GL_TEXTURE_MANAGER
            PUSH("GLTextureManager: create sampler [name: '%s'][ref: %u]", sampler.getName(), sampler.getReferenceCount());
#endif
        }

        return mSamplers.getLast();
    }

    ISampler* GLTextureManager::findSampler(const char *name)
    {
        for (auto current = mSamplers.iterate(); current != nullptr; current = mSamplers.next())
        {
            if (current->mResourceName == name)
            {
                return current;
            }
        }

        return nullptr;
    }

    ISampler* GLTextureManager::getSampler(const char *name)
    {
        ISampler* found = findSampler(name);

        if (found)
        {
            found->addReference();
#if PROFILE_GL_TEXTURE_MANAGER
            PUSH("GLTextureManager: find sampler [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
#endif
            return found;
        }

        return nullptr;
    }

    ISampler* GLTextureManager::getSamplerLinear()
    {
        mSamplerLinear->addReference();
        return mSamplerLinear;
    }

    ISampler* GLTextureManager::getSamplerNearest()
    {
        mSamplerNearest->addReference();
        return mSamplerNearest;
    }

    uint32 GLTextureManager::getMemoryUsage()
    {
        return sizeof(GLTextureManager)   +
               mTextures.getMemoryUsage() +
               mSamplers.getMemoryUsage() ;

    }

} // namespace Berserk::Resources