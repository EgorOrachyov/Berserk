//
// Created by Egor Orachyov on 01.03.2019.
//

#include "Manager/GLTextureManager.h"

namespace Berserk
{

    namespace Resources
    {

        void GLTextureManager::initialize(Importers::IImageImporter *importer, const char *path)
        {
            {
                new(&mTextures) LinkedList<GLTexture>(INITIAL_TEXTURES_COUNT);
                new(&mSamplers) LinkedList<GLSampler>(INITIAL_SAMPLERS_COUNT);
                new(&mTexturesPath) CString(path);

                mImageImporter = importer;
            }

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
                CPath filepath(mTexturesPath.get());
                filepath += "/";
                mDefaultTexture = loadTexture(filepath.get(), "Default/DefaultTexture.png");
                mDefaultHelperTexture = loadTexture(filepath.get(), "Default/DefaultHelperTexture.png");
            }
        }

        void GLTextureManager::release()
        {
            {
                // Explicit deletions for textures in the manager,
                // Which references were not decreased to the 0
                // (Avoids memory leaks)

                for (auto current = mTextures.iterate(); current != nullptr; current = mTextures.next())
                {
                    PUSH("GLTextureManager: release texture [name: '%s']", current->getName());

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
                    PUSH("GLTextureManager: release sampler [name: '%s']", current->getName());

                    current->mReferenceCount = 0;
                    current->release();
                }
            }

            delete(&mTextures);
            delete(&mSamplers);
            delete(&mTexturesPath);
        }

        void GLTextureManager::renameTexture(ITexture *texture, const char *name)
        {
            PUSH("GLTextureManager: rename texture [old name: '%s'][new name: '%s']", texture->getName(), name);

            auto renamed = dynamic_cast<GLTexture*>(texture);
            renamed->mResourceName = name;
        }

        void GLTextureManager::renameSampler(ISampler *sampler, const char *name)
        {
            PUSH("GLTextureManager: rename sampler [old name: '%s'][new name: '%s']", sampler->getName(), name);

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

                PUSH("GLTextureManager: delete texture [name: '%s']", name.get());
                mTextures.remove((GLTexture*)texture);
            }
        }

        void GLTextureManager::deleteSampler(ISampler *sampler)
        {
            CText name(sampler->getName());
            sampler->release();

            if (sampler->getReferenceCount() == 0)
            {
                PUSH("GLTextureManager: delete sampler [name: '%s']", name.get());
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

                PUSH("GLTextureManager: create texture [name: '%s'][ref: %u]", texture.getName(), texture.getReferenceCount());
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
                PUSH("GLTextureManager: find texture [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
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
                PUSH("GLTextureManager: load texture [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
                return found;
            }

            {
                CPath filename(mTexturesPath.get());
                filename.replace(CPath("{TEXTURES}"), CPath(mTexturesPath.get()));
                filename += name;

                Importers::IImageImporter::ImageData data;

                auto loaded = mImageImporter->import(filename.get(), data);

                if (!loaded)
                {
                    WARNING("GLTextureManager: failed to load texture [name: '%s']", filename.get());
                    return getDefaultHelperTexture();
                }

                GLTexture texture;
                texture.initialize(name);
                texture.addReference();
                texture.create(data.width, data.height, data.storageFormat, data.pixelFormat, data.pixelType, data.buffer, true);
                texture.mSampler = getSamplerLinear();

                mTextures += texture;

                PUSH("GLTextureManager: load texture [name: '%s'][ref: %u]", texture.getName(), texture.getReferenceCount());

                return mTextures.getLast();
            }

        }

        ITexture* GLTextureManager::loadTextureFromXML(const char *name, XMLNode &node)
        {
            // todo
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

                PUSH("GLTextureManager: create sampler [name: '%s'][ref: %u]", sampler.getName(), sampler.getReferenceCount());
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
                PUSH("GLTextureManager: find sampler [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
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

    } // namespace Resources

} // namespace Berserk