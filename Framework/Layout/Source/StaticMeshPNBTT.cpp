//
// Created by Egor Orachyov on 29.07.2018.
//

#include "Render/RenderSystem.h"
#include "Managers/SceneManager.h"
#include "Objects/Mesh/StaticMeshPNBTT.h"

namespace Berserk
{

    StaticMeshPNBTT::StaticMeshPNBTT(const CStaticString &name, ArrayList<VertexPNBTT> &vertices, ArrayList<UINT16> &indices)
    {
        mGPUBuffer = RenderSystem::getRenderSystemRef().createGPUBuffer(name);

        mGPUBuffer->init();
        mGPUBuffer->attachData(vertices);
        mGPUBuffer->attachIndices(indices);

        if (!mGPUBuffer->validate())
        {
            ERROR("Cannot create GPU buffer for static PNBTT mesh");
            SAFE_DELETE(mGPUBuffer);
            gSceneManager->askErrorClose();
        }
    }

    GPUBuffer *StaticMeshPNBTT::getGPUBuffer()
    {
        return mGPUBuffer;
    }

} // namespace Berserk