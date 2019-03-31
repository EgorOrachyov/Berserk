//
// Created by Egor Orachyov on 01.03.2019.
//

#ifndef BERSERK_GLTEXTUREMANAGER_H
#define BERSERK_GLTEXTUREMANAGER_H

#include "Platform/GLTexture.h"
#include "Platform/GLSampler.h"
#include "Containers/LinkedList.h"
#include "Managers/ITextureManager.h"
#include "Strings/String.h"

namespace Berserk::Resources
{

    /**
     * OpenGL platform texture manager implementation
     */
    class ENGINE_API GLTextureManager : public ITextureManager
    {
    public:

        /**
         * Explicit initialization of manager (must be invoked)
         * @param importer Pointer to initialized engine image importer
         *                 to load textures from HHD
         * @param path Path to the !folder! with engine/game textures
         *             resource folder
         */
        GLTextureManager(Importers::IImageImporter* importer, const char *path);

        /** De-initialize manager */
        ~GLTextureManager() override;

        /** @copydoc ITextureManager::renameTexture() */
        void renameTexture(ITexture* texture, const char* name) override;

        /** @copydoc ITextureManager::renameSampler() */
        void renameSampler(ISampler *sampler, const char *name) override;

        /** @copydoc ITextureManager::saveTexture() */
        void saveTexture(ITexture* texture, const char* path) override;

        /** @copydoc ITextureManager::bindSampler() */
        void bindSampler(ITexture* texture, ISampler* sampler) override;

        /** @copydoc ITextureManager::deleteTexture() */
        void deleteTexture(ITexture* texture) override;

        /** @copydoc ITextureManager::deleteSampler() */
        void deleteSampler(ISampler* sampler) override;

        /** @copydoc ITextureManager::createTexture() */
        ITexture* createTexture(const char* name) override;

        /** @copydoc ITextureManager::findTexture() */
        ITexture* findTexture(const char* name) override;

        /** @copydoc ITextureManager::getTexture() */
        ITexture* getTexture(const char* name) override;

        /** @copydoc ITextureManager::loadTexture() */
        ITexture* loadTexture(const char *path, const char *name) override;

        /** @copydoc ITextureManager::loadTextureFromXML() */
        ITexture* loadTextureFromXML(const char* name, XMLNode& node) override;

        /** @copydoc ITextureManager::copyTexture() */
        ITexture* copyTexture(ITexture* texture) override;

        /** @copydoc ITextureManager::getDefaultTexture() */
        ITexture* getDefaultTexture() override;

        /** @copydoc ITextureManager::getDefaultHelperTexture() */
        ITexture* getDefaultHelperTexture() override;

        /** @copydoc ITextureManager::createSampler() */
        ISampler* createSampler(const char *name) override;

        /** @copydoc ITextureManager::findSampler() */
        ISampler* findSampler(const char *name) override;

        /** @copydoc ITextureManager::getSampler() */
        ISampler* getSampler(const char* name) override;

        /** @copydoc ITextureManager::getSamplerLinear() */
        ISampler* getSamplerLinear() override;

        /** @copydoc ITextureManager::getSamplerNearest() */
        ISampler* getSamplerNearest() override;

        /** @copydoc ITextureManager::getMemoryUsage() */
        uint32 getMemoryUsage() override;

        /** @copydoc ITextureManager::getMemoryUsage() */
        void getMemoryUsage(MemorySizer* sizer) override;

    private:

        /** Number of textures to preallocate in buffer (and the expand by that value) */
        static const uint32 INITIAL_TEXTURES_COUNT = 100;

        /** Number of samplers to preallocate in buffer (and the expand by that value) */
        static const uint32 INITIAL_SAMPLERS_COUNT = 10;

        LinkedList<GLTexture> mTextures;
        LinkedList<GLSampler> mSamplers;

        ITexture* mDefaultTexture;
        ITexture* mDefaultHelperTexture;

        ISampler* mSamplerLinear;
        ISampler* mSamplerNearest;

        Importers::IImageImporter* mImageImporter;

        CString mTexturesPath;

    };

} // namespace Berserk::Resources

#endif //BERSERK_GLTEXTUREMANAGER_H