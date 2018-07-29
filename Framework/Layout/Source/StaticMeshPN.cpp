//
// Created by Egor Orachyov on 29.07.2018.
//

#include "System/RenderSystem.h"
#include "Managers/SceneManager.h"
#include "Objects/Mesh/StaticMeshPN.h"

namespace Berserk
{

    StaticMeshPN::StaticMeshPN(const CStaticString &name, ArrayList<VertexPN> &vertices, ArrayList<UINT16> &indices)
    {
        mGPUBuffer = RenderSystem::getRenderSystemRef().createGPUBuffer(name);

        mGPUBuffer->init();
        mGPUBuffer->attachData(vertices);
        mGPUBuffer->attachIndices(indices);

        if (!mGPUBuffer->validate())
        {
            ERROR("Cannot create GPU buffer for static PN mesh");
            SAFE_DELETE(mGPUBuffer);
            gSceneManager->askErrorClose();
        }
    }

    GPUBuffer *StaticMeshPN::getGPUBuffer()
    {
        return mGPUBuffer;
    }
    
} // namespace Berserk