//
// Created by Egor Orachyov on 10.08.2018.
//

#include "Managers/GLRenderMeshManager.h"

namespace Berserk
{

    GLRenderMeshManager::GLRenderMeshManager()
    {
        mMeshList.init(BUFFER_SIZE_64);
    }

    void GLRenderMeshManager::destroy()
    {
        mMeshList.empty();
    }

    UINT32 GLRenderMeshManager::getMemoryUsage()
    {
        UINT32 memUsage = mMeshList.getSize() * sizeof(mMeshList.getSizeOfNode());
        return memUsage;
    }

    RenderMesh* GLRenderMeshManager::getRenderMesh(UINT32 id)
    {
        return &mMeshList.get(id);
    }

    RenderMesh* GLRenderMeshManager::getRenderMesh(const CStaticString& name)
    {
        mMeshList.iterate(true);
        while (mMeshList.iterate())
        {
            if (mMeshList.getCurrent().getName() == name)
            {
                return &mMeshList.getCurrent();
            }
        }

        return nullptr;
    }

    RenderMesh* GLRenderMeshManager::createRenderMesh(const CStaticString& name)
    {
        RenderMesh* found = getRenderMesh(name);
        if (found != nullptr)
        {
            found->addReference();
            return found;
        }

        GLRenderMesh mesh;
        mesh.setName(name);
        mesh.addReference();
        mMeshList.add(mesh);
        return &mMeshList.getLast();
    }

    RenderMesh* GLRenderMeshManager::renameRenderMesh(const CStaticString& oldName, const CStaticString& newName)
    {
        mMeshList.iterate(true);
        while (mMeshList.iterate())
        {
            if (mMeshList.getCurrent().getName() == oldName)
            {
                GLRenderMesh* mesh = &mMeshList.getCurrent();
                mesh->setName(newName);
                return mesh;
            }
        }

        return nullptr;
    }

    bool GLRenderMeshManager::deleteRenderMesh(UINT32 id)
    {
        GLRenderMesh* mesh = &mMeshList.get(id);

        if (mesh == nullptr)
        {
            return false;
        }

        mesh->release();
        if (mesh->getReferences() == 0)
        {
            mMeshList.remove(*mesh);
        }

        return true;
    }

    bool GLRenderMeshManager::deleteRenderMesh(const CStaticString& name)
    {
        GLRenderMesh* material = nullptr;
        mMeshList.iterate(true);
        while (mMeshList.iterate())
        {
            if (mMeshList.getCurrent().getName() == name)
            {
                material = &mMeshList.getCurrent();
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
            mMeshList.remove(*material);
        }

        return true;
    }

    bool GLRenderMeshManager::deleteRenderMesh(RenderMesh* toDelete)
    {
        GLRenderMesh* material = nullptr;
        mMeshList.iterate(true);
        while (mMeshList.iterate())
        {
            if (mMeshList.getCurrent().getName() == toDelete->getName())
            {
                material = &mMeshList.getCurrent();
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
            mMeshList.remove(*material);
        }

        return true;
    }
    
} // namespace Berserk