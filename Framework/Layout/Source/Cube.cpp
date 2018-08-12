//
// Created by Egor Orachyov on 16.07.2018.
//

#include "Shapes/Cube.h"
#include "Strings/UtilityString.h"

namespace Berserk
{

    RenderMesh * Cube::create(UINT32 mask, FLOAT32 size, const CStaticString& name)
    {
        //      v4 ------ v7
        //     /|         /|
        //    / |        / |
        //   /  |       /  |
        //  /   v5 ----/- v6
        // v0 ------ v3   /
        // |   /      |  /
        // |  /       | /
        // | /        |/
        // v1 ------ v2

        FLOAT32 w = size / 2;

        Vector3f n0 = Vector3f(1,0,0), n1 = Vector3f(-1,0,0),
                 n2 = Vector3f(0,1,0), n3 = Vector3f(0,-1,0),
                 n4 = Vector3f(0,0,1), n5 = Vector3f(0,0,-1);

        Vector3f v0 = Vector3f(-w,w,w),  v1 = Vector3f(-w,-w,w),
                 v2 = Vector3f(w,-w,w),  v3 = Vector3f(w,w,w),
                 v4 = Vector3f(-w,w,-w), v5 = Vector3f(-w,-w,-w),
                 v6 = Vector3f(w,-w,-w), v7 = Vector3f(w,w,-w);

        const UINT32 index_count = 36;
        UINT16  i[index_count] = {
                0,1,2,3,4,5,
                6,7,8,9,10,11,
                12,13,14,15,16,17,
                18,19,20,21,22,23,
                24,25,26,27,28,29,
                30,31,32,33,34,35
        };

        if (mask == MeshType::MT_PN)
        {
            const UINT32 data_count = 36;
            VertexPN data[data_count] = {
                    {v0,n4},{v1,n4},{v2,n4},{v2,n4},{v3,n4},{v0,n4},
                    {v3,n0},{v2,n0},{v6,n0},{v6,n0},{v7,n0},{v3,n0},
                    {v7,n5},{v6,n5},{v5,n5},{v5,n5},{v4,n5},{v7,n5},
                    {v4,n1},{v5,n1},{v1,n1},{v1,n1},{v0,n1},{v4,n1},
                    {v4,n2},{v0,n2},{v3,n2},{v3,n2},{v7,n2},{v4,n2},
                    {v6,n3},{v2,n3},{v1,n3},{v1,n3},{v5,n3},{v6,n3}
            };

            RenderMesh* mesh = gRenderSystem->getRenderMeshManagerPtr()->createRenderMesh(name);
            mesh->setType(MeshType::MT_PN);
            mesh->addGeometryInfo(data, data_count);
            mesh->addGeometryInfo(i, index_count);
            return mesh;
        }
    }

} // namespace Berserk