//
// Created by Egor Orachyov on 05.04.2019.
//

#include "Components/StaticMeshComponent.h"

namespace Berserk::EngineSystem
{

    StaticMeshComponent::StaticMeshComponent(const IObjectInitializer &objectInitializer)
            : IPrimitiveComponent(objectInitializer),
              mSubMeshSet(INITIAL_MESH_COUNT, objectInitializer.getAllocator()),
              mUsedMaterials(INITIAL_MATERIAL_COUNT, objectInitializer.getAllocator())
    {

    }

    StaticMeshComponent::~StaticMeshComponent()
    {
        /** Free Render System resources [Materials] */
        for (uint32 i = 0; i < mUsedMaterials.getSize(); i++)
        {
            getMaterialManager()->deleteMaterial(mUsedMaterials[i]);
        }

        /** Free Render System resources [Geometry buffers] */
        for (uint32 i = 0; i < mSubMeshSet.getSize(); i++)
        {
            getBufferManager()->deleteGPUBuffer(mSubMeshSet[i].mGeometryBuffer);
        }
    }

} // namespace Berserk::EntitySystem