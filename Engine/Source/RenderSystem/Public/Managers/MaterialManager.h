//
// Created by Egor Orachyov on 24.03.2019.
//

#ifndef BERSERK_MATERIALMANAGER_H
#define BERSERK_MATERIALMANAGER_H

#include "Containers/LinkedList.h"
#include "Managers/IMaterialManager.h"
#include "Base/Material.h"

namespace Berserk::Resources
{

#ifndef PROFILE_MATERIAL_MANAGER
    #define PROFILE_MATERIAL_MANAGER 0
#endif

    class ENGINE_API MaterialManager : public IMaterialManager
    {
    public:

        /**
         * Initialize Material manager with texture
         * manager to import needed texture maps for material and
         * path to the folder with engine materials
         */
        MaterialManager(ITextureManager *manager, const char *path);

        /** De-initialize and delete all the loaded materials */
        ~MaterialManager() override;

        /** @copydoc IMaterialManager::renameMaterial() */
        void renameMaterial(IMaterial* material, const char* name) override;

        /** @copydoc IMaterialManager::saveMaterial() */
        void saveMaterial(IMaterial* material, XMLNode& node) override;

        /** @copydoc IMaterialManager::deleteMaterial() */
        void deleteMaterial(IMaterial* material) override;

        /** @copydoc IMaterialManager::createMaterial() */
        IMaterial* createMaterial(const char* name) override;

        /** @copydoc IMaterialManager::findMaterial() */
        IMaterial* findMaterial(const char* name) override;

        /** @copydoc IMaterialManager::getMaterial() */
        IMaterial* getMaterial(const char* name) override;

        /** @copydoc IMaterialManager::loadMaterial() */
        IMaterial* loadMaterial(const char* path) override;

        /** @copydoc IMaterialManager::loadMaterialFromXML() */
        IMaterial* loadMaterialFromXML(const char* name, XMLNode& node) override;

        /** @copydoc IMaterialManager::copyMaterial() */
        IMaterial* copyMaterial(IMaterial* material) override;

        /** @copydoc IMaterialManager::getDefaultMaterial() */
        IMaterial* getDefaultMaterial() override;

        /** @copydoc IMaterialManager::getDefaultHelperMaterial() */
        IMaterial* getDefaultHelperMaterial() override;

        /** @copydoc IMaterialManager::getDefaultTerrainMaterial() */
        IMaterial* getDefaultTerrainMaterial() override;

        /** @copydoc IMaterialManager::getMemoryUsage() */
        uint32 getMemoryUsage() override;

        /** @copydoc IMaterialManager::getMemoryUsage() */
        void getMemoryUsage(MemorySizer* sizer) override;

    private:

        static const uint32 INITIAL_MATERIALS_COUNT = 64;

        LinkedList<Material> mMaterials;

        IMaterial* mDefaultMaterial;
        IMaterial* mDefaultHelperMaterial;
        IMaterial* mDefaultTerrainMaterial;

        CString mMaterialsPath;

        ITextureManager* mTextureManager;

    };

} // namespace Berserk::Resources

#endif //BERSERK_MATERIALMANAGER_H