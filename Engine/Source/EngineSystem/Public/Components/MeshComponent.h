//
// Created by Egor Orachyov on 06.04.2019.
//

#ifndef BERSERK_MESHCOMPONENT_H
#define BERSERK_MESHCOMPONENT_H

#include <Foundation/IObjectMacros.h>
#include <Managers/IBufferManager.h>
#include <Foundation/IMaterial.h>
#include <Platform/VertexTypes.h>

namespace Berserk::Engine
{

    /**
     * Simple container for storing pairs of geometry and material
     * to pass it to render queue
     */
    class ENGINE_API MeshComponent final
    {
    public:

        /** Delete new/delete functions */
        GENARATE_NEW_DELETE(MeshComponent);

        /** Null mesh component */
        MeshComponent() { /** Set to null */}

        /** Copy-assign */
        MeshComponent(Resources::IGPUBuffer* buffer, Resources::IMaterial* material)
                : mGeometryBuffer(buffer),
                  mRenderMaterial(material)
        {
            FAIL(buffer, "Null pointer IGPUBuffer resource");
            FAIL(material, "Null pointer IMaterial resource");
        }

        /** Do nothing (resource must be freed by owner of this mesh) */
        ~MeshComponent() = default;

        /**
         * Allows to get information about raw mesh data
         * @param[out] type  Type of vertices
         * @param[out] count Count of vertices
         */
        void getGeometryData(Resources::IGPUBuffer::VertexType &type, uint32 &count)
        {
            type = mGeometryBuffer->getVertexType();
            count = mGeometryBuffer->getVertexCount();
        }

        /** How to compare two mesh to sort in order of the materials apply */
        static bool comparePredicate(const MeshComponent* mesh1, const MeshComponent* mesh2)
        {
            return (mesh1->mRenderMaterial > mesh2->mRenderMaterial);
        }

    public:

        /** Buffer which contains polygons info for this mesh */
        class Resources::IGPUBuffer* mGeometryBuffer = nullptr;

        /** Material defines how to draw its geometry */
        class Resources::IMaterial* mRenderMaterial = nullptr;

    };

}

#endif //BERSERK_MESHCOMPONENT_H
