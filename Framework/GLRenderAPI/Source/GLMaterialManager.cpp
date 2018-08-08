//
// Created by Egor Orachyov on 08.08.2018.
//

#include "Managers/GLMaterialManager.h"
#include "Strings/UtilityString.h"

namespace Berserk
{

    GLMaterialManager::GLMaterialManager()
    {
        mMaterialList.init();

        GLMaterial defaultMat;
        defaultMat.setType(MaterialType::MT_BASIC);
        defaultMat.setName(CNAME("DefaultMaterial"));
        defaultMat.setAmbientComponent(Vector3f(0.2));
        defaultMat.setDiffuseComponent(Vector3f(0.2));
        defaultMat.setSpecularComponent(Vector3f(0.2));
        defaultMat.setShininess(8);

        mMaterialList.add(defaultMat);
    }

    void GLMaterialManager::destroy()
    {
        mMaterialList.empty();
    }

    UINT32 GLMaterialManager::getMemoryUsage() const
    {
        UINT32 memUsage = sizeof(GLMaterialManager);
        if (mMaterialList.getSize())
        {
            memUsage += mMaterialList.getSize() * mMaterialList.getFirst().getMemoryUsage();
        }

        return memUsage;
    }

    Material* GLMaterialManager::getMaterial(UINT32 id)
    {
        return &mMaterialList.get(id);
    }

    Material* GLMaterialManager::getMaterial(const CStaticString& name)
    {
        mMaterialList.iterate(true);
        while (mMaterialList.iterate())
        {
            if (mMaterialList.getCurrent().getName() == name)
            {
                return &mMaterialList.getCurrent();
            }
        }

        return nullptr;
    }

    Material* GLMaterialManager::createMaterial(const CStaticString& name)
    {
        GLMaterial material;
        material.setName(name);
        mMaterialList.add(material);
        return &mMaterialList.getLast();
    }

    Material* GLMaterialManager::renameMaterial(const CStaticString& oldName, const CStaticString& newName)
    {
        mMaterialList.iterate(true);
        while (mMaterialList.iterate())
        {
            if (mMaterialList.getCurrent().getName() == oldName)
            {
                GLMaterial* current = &mMaterialList.getCurrent();
                current->setName(newName);
                return current;
            }
        }

        return nullptr;
    }

    Material* GLMaterialManager::loadMaterialFromMTL(const CString &file)
    {
        // todo: add mtl loading
    }

    Material* GLMaterialManager::loadMaterialFromMTL(const CStaticString &file)
    {
        // todo: add mtl loading
    }

    Material* GLMaterialManager::loadMaterialFromXML(const CString& file)
    {
        // todo: add xml loading
    }

    Material* GLMaterialManager::loadMaterialFromXML(const CStaticString& file)
    {
        // todo: add xml loading
    }

    Material* GLMaterialManager::getDefaultMaterial() const
    {
        return &mMaterialList.getFirst();
    }

    bool GLMaterialManager::deleteMaterial(UINT32 id)
    {
        GLMaterial* material = &mMaterialList.get(id);

        if (material == nullptr)
        {
            return false;
        }

        material->release();
        if (material->getReferences() == 0)
        {
            mMaterialList.remove(*material);
        }

        return true;
    }

    bool GLMaterialManager::deleteMaterial(const CStaticString& name)
    {
        GLMaterial* material = nullptr;
        mMaterialList.iterate(true);
        while (mMaterialList.iterate())
        {
            if (mMaterialList.getCurrent().getName() == name)
            {
                material = &mMaterialList.getCurrent();
                break;
            }
        }

        if (material == nullptr)
        {
            return false;
        }

        material->release();
        if (material->getReferences() == 0)
        {
            mMaterialList.remove(*material);
        }

        return true;
    }

    bool GLMaterialManager::deleteMaterial(Material* toDelete)
    {
        GLMaterial* material = nullptr;
        mMaterialList.iterate(true);
        while (mMaterialList.iterate())
        {
            if (mMaterialList.getCurrent().getName() == toDelete->getName())
            {
                material = &mMaterialList.getCurrent();
                break;
            }
        }

        if (material == nullptr)
        {
            return false;
        }

        material->release();
        if (material->getReferences() == 0)
        {
            mMaterialList.remove(*material);
        }

        return true;
    }

} // namespace Berserk