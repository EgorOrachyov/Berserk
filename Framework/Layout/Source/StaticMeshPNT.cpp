//
// Created by Egor Orachyov on 28.07.2018.
//

#include "System/RenderSystem.h"
#include "Managers/SceneManager.h"
#include "Objects/Mesh/StaticMeshPNT.h"

namespace Berserk
{

    StaticMeshPNT::StaticMeshPNT(const CStaticString &name, ArrayList<VertexPNT> &vertices, ArrayList<UINT16> &indices)
    {
        mGPUBuffer = RenderSystem::getRenderSystemRef().createGPUBuffer(name);

        mGPUBuffer->init();
        mGPUBuffer->attachData(vertices);
        mGPUBuffer->attachIndices(indices);

        if (!mGPUBuffer->validate())
        {
            ERROR("Cannot create GPU buffer for static PNT mesh");
            SAFE_DELETE(mGPUBuffer);
            gSceneManager->askForErrorClose();
        }
    }

    GPUBuffer *StaticMeshPNT::getGPUBuffer()
    {
        return mGPUBuffer;
    }

} // namespace Berserk