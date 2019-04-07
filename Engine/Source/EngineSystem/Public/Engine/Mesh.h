//
// Created by Egor Orachyov on 06.04.2019.
//

#ifndef BERSERK_MESH_H
#define BERSERK_MESH_H

#include <Managers/IBufferManager.h>
#include <Foundation/IMaterial.h>

namespace Berserk::EngineSystem
{

    /**
     * Simple container for storing pairs of geometry and material
     * to pass it to render queue
     */
    class ENGINE_API Mesh final
    {
    public:

        Mesh(Resources::IGPUBuffer* buffer, Resources::IMaterial* material)
                : mGeometryBuffer(buffer),
                  mRenderMaterial(material)
        {
            FAIL(buffer, "Null pointer IGPUBuffer resource");
            FAIL(material, "Null pointer IMaterial resource");
        }

        ~Mesh() = default;

        /** How to compare two mesh to sort in order of the materials apply */
        static bool comparePredicate(const Mesh* mesh1, const Mesh* mesh2)
        {
            return (mesh1->mRenderMaterial > mesh2->mRenderMaterial);
        }

    public:

        /** Buffer which contains polygons info for this mesh */
        Resources::IGPUBuffer* mGeometryBuffer;

        /** Material defines how to draw its geometry */
        Resources::IMaterial*  mRenderMaterial;

    };

}

#endif //BERSERK_MESH_H
