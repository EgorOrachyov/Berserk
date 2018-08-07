//
// Created by Egor Orachyov on 29.07.2018.
//

#include "Render/RenderSystem.h"
#include "Managers/SceneManager.h"
#include "Objects/Mesh/StaticMesh.h"

namespace Berserk
{

    StaticMesh::StaticMesh(const CStaticString &name, ArrayList<Vertex> &vertices, ArrayList<UINT16> &indices)
    {
        mGPUBuffer = RenderSystem::getRenderSystemRef().createGPUBuffer(name);

        mGPUBuffer->init();
        mGPUBuffer->attachData(vertices);
        mGPUBuffer->attachIndices(indices);

        if (!mGPUBuffer->validate())
        {
            ERROR("Cannot create GPU buffer for static mesh");
            SAFE_DELETE(mGPUBuffer);
            gSceneManager->askErrorClose();
        }
    }

    GPUBuffer *StaticMesh::getGPUBuffer()
    {
        return mGPUBuffer;
    }
    
} // namespace Berserk