//
// Created by Egor Orachyov on 24.03.2019.
//

#include "Managers/MaterialManager.h"
#include "XMLDocument.h"
#include "Helpers/MaterialManagerHelper.h"

namespace Berserk::Resources
{

    MaterialManager::MaterialManager(ITextureManager *manager, const char *path)
            : mMaterials(INITIAL_MATERIALS_COUNT),
              mMaterialsPath(path),
              mTextureManager(manager)
    {
        FAIL(manager, "Null pointer texture manager");
        FAIL(manager, "Null pointer material folder path");

        mDefaultMaterial = loadMaterial("{MATERIALS}/Default/DefaultMaterial.xml");
        mDefaultHelperMaterial = loadMaterial("{MATERIALS}/Default/HelperMaterial.xml");
        mDefaultTerrainMaterial = loadMaterial("{MATERIALS}/Default/TerrainMaterial.xml");

        PUSH("GLShaderManager: initialize");
    }

    MaterialManager::~MaterialManager() 
    {
        {
            // Explicit deletions for materials in the manager,
            // Which references were not decreased to the 0
            // (Avoids memory leaks)

            for (auto current = mMaterials.iterate(); current != nullptr; current = mMaterials.next())
            {
#if PROFILE_MATERIAL_MANAGER
                PUSH("MaterialManager: release material [name: '%s']", current->getName());
#endif

                current->mReferenceCount = 0;
                current->release();
            }
        }

        PUSH("MaterialManager: de-initialize");
    }

    void MaterialManager::renameMaterial(IMaterial *material, const char *name)
    {
#if PROFILE_MATERIAL_MANAGER
        PUSH("MaterialManager: rename material [old name: '%s'][new name: '%s']", material->getName(), name);
#endif

        auto renamed = dynamic_cast<Material*>(material);
        renamed->mResourceName = name;
    }

    void MaterialManager::saveMaterial(IMaterial *material, XMLNode &node)
    {
        // todo
    }

    void MaterialManager::deleteMaterial(IMaterial *material)
    {
        CText name(material->getName());
        material->release();

        if (material->getReferenceCount() == 0)
        {
#if PROFILE_MATERIAL_MANAGER
            PUSH("MaterialManager: delete material [name: '%s']", name.get());
#endif
            mMaterials.remove((Material*)material);
        }
    }

    IMaterial* MaterialManager::createMaterial(const char *name)
    {
        IMaterial* found = findMaterial(name);

        if (found)
        {
            WARNING("MaterialManager: material already exist [name: '%s']", name);
            return nullptr;
        }

        auto material = new(mMaterials.preallocate()) Material(name, mTextureManager);

        {
            material->addReference();
#if PROFILE_MATERIAL_MANAGER
            PUSH("MaterialManager: create material [name: '%s'][ref: %u]", material->getName(), material->getReferenceCount());
#endif
        }

        return material;
    }

    IMaterial* MaterialManager::findMaterial(const char *name)
    {
        for (auto current = mMaterials.iterate(); current != nullptr; current = mMaterials.next())
        {
            if (current->mResourceName == name)
            {
                return current;
            }
        }

        return nullptr;
    }

    IMaterial* MaterialManager::getMaterial(const char *name)
    {
        IMaterial* found = findMaterial(name);

        if (found)
        {
            found->addReference();
#if PROFILE_MATERIAL_MANAGER
            PUSH("MaterialManager: find material [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
#endif
            return found;
        }

        return nullptr;
    }

    IMaterial* MaterialManager::loadMaterial(const char *path)
    {
        CPath filename(path);
        filename = filename.replace(CPath("{MATERIALS}"), CPath(mMaterialsPath.get()));

        IMaterial* last = nullptr;
        XMLDocument matinfo(filename.get(), ".xml");

        for (auto materials = matinfo.getFirst(); !materials.isEmpty(); materials = materials.getNext())
        {
            last = loadMaterialFromXML(materials.getAttribute("name").getValue(), materials);
        }

        return last;
    }

    IMaterial* MaterialManager::loadMaterialFromXML(const char *name, XMLNode &node)
    {
        if (name != nullptr)
        {
            auto found = findMaterial(name);

            if (found)
            {
                found->addReference();
#if PROFILE_MATERIAL_MANAGER
                PUSH("MaterialManager: find material [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
#endif
                return found;
            }
        }

        const char* matname = node.getAttribute("name").getValue();

        {
            auto found = findMaterial(matname);

            if (found)
            {
                found->addReference();
#if PROFILE_MATERIAL_MANAGER
                PUSH("MaterialManager: find material [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
#endif
                return found;
            }
        }

        auto material = new(mMaterials.preallocate()) Material(matname, mTextureManager);
        auto success = MaterialManagerHelper::import(material, node, mMaterialsPath, mTextureManager);

        if (!success)
        {
            WARNING("Cannot load material program from xml node [name: '%s']", matname);

            material->release();
            mMaterials.remove(material);
            return getDefaultHelperMaterial();
        }

        material->addReference();
        return material;
    }

    IMaterial* MaterialManager::copyMaterial(IMaterial *material)
    {
        material->addReference();
        return material;
    }

    IMaterial* MaterialManager::getDefaultMaterial()
    {
        mDefaultMaterial->addReference();
        return mDefaultMaterial;
    }

    IMaterial* MaterialManager::getDefaultHelperMaterial()
    {
        mDefaultHelperMaterial->addReference();
        return mDefaultHelperMaterial;
    }

    IMaterial* MaterialManager::getDefaultTerrainMaterial()
    {
        mDefaultTerrainMaterial->addReference();
        return mDefaultTerrainMaterial;
    }

    uint32 MaterialManager::getMemoryUsage()
    {
        return sizeof(MaterialManager)    +
               mMaterials.getMemoryUsage();
    }

    void MaterialManager::getMemoryUsage(MemorySizer *sizer)
    {
        auto root = sizer->addObject("MaterialManager", getMemoryUsage());

        for (auto current = mMaterials.iterate(); current != nullptr; current = mMaterials.next())
        {
            sizer->addChild(root, current->getName(), current->getMemoryUsage(), current->getGPUMemoryUsage());
        }
    }

} // namespace Berserk::Resources