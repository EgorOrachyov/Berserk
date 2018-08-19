//
// Created by Egor Orachyov on 16.07.2018.
//

#include "Shapes/Plane.h"
#include "Strings/UtilityString.h"

namespace Berserk
{

    RenderMesh* Plane::create(UINT32 mask, FLOAT32 size, const CStaticString& name)
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
        FLOAT32 d = 0.05;

        Vector3f n0 = Vector3f(1,0,0), n1 = Vector3f(-1,0,0),
                 n2 = Vector3f(0,1,0), n3 = Vector3f(0,-1,0),
                 n4 = Vector3f(0,0,1), n5 = Vector3f(0,0,-1);

        Vector3f v0 = Vector3f(-w,0,w),  v1 = Vector3f(-w,-d,w),
                 v2 = Vector3f(w,-d,w),  v3 = Vector3f(w,0,w),
                 v4 = Vector3f(-w,0,-w), v5 = Vector3f(-w,-d,-w),
                 v6 = Vector3f(w,-d,-w), v7 = Vector3f(w,0,-w);

        Vector2f t0 = Vector2f(0,1), t1 = Vector2f(0,0),
                t2 = Vector2f(1,0), t3 = Vector2f(1.0,1.0);


        Vector3f tg0 = Vector3f(1,0,0),  tg1 = Vector3f(0,0,-1),
                tg2 = Vector3f(-1,0,0), tg3 = Vector3f(0,0,1),
                tg4 = Vector3f(1,0,0),  tg5 = Vector3f(-1,0,0);

        Vector3f bg0 = Vector3f(0,1,0),  bg1 = Vector3f(0,1,0),
                bg2 = Vector3f(0,1,0),  bg3 = Vector3f(0,1,0),
                bg4 = Vector3f(0,0,-1), bg5 = Vector3f(0,0,-1);

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

        if (mask == MeshType::MT_PNT)
        {
            const UINT32 data_count = 36;
            VertexPNT data[data_count] = {
                    {v0,n4,t0},{v1,n4,t1},{v2,n4,t2},{v2,n4,t2},{v3,n4,t3},{v0,n4,t0},
                    {v3,n0,t0},{v2,n0,t1},{v6,n0,t2},{v6,n0,t2},{v7,n0,t3},{v3,n0,t0},
                    {v7,n5,t0},{v6,n5,t1},{v5,n5,t2},{v5,n5,t2},{v4,n5,t3},{v7,n5,t0},
                    {v4,n1,t0},{v5,n1,t1},{v1,n1,t2},{v1,n1,t2},{v0,n1,t3},{v4,n1,t0},
                    {v4,n2,t0},{v0,n2,t1},{v3,n2,t2},{v3,n2,t2},{v7,n2,t3},{v4,n2,t0},
                    {v6,n3,t0},{v2,n3,t1},{v1,n3,t2},{v1,n3,t2},{v5,n3,t3},{v6,n3,t0}
            };

            RenderMesh* mesh = gRenderSystem->getRenderMeshManagerPtr()->createRenderMesh(name);
            mesh->setType(MeshType::MT_PNT);
            mesh->addGeometryInfo(data, data_count);
            mesh->addGeometryInfo(i, index_count);
            return mesh;
        }

        if (mask == MeshType::MT_PNTBT)
        {
            const UINT32 data_count = 36;
            VertexPNTBT data[data_count] = {
                    {v0,n4,t0,tg0,bg0},{v1,n4,t1,tg0,bg0},{v2,n4,t2,tg0,bg0},{v2,n4,t2,tg0,bg0},{v3,n4,t3,tg0,bg0},{v0,n4,t0,tg0,bg0},
                    {v3,n0,t0,tg1,bg1},{v2,n0,t1,tg0,bg0},{v6,n0,t2,tg0,bg0},{v6,n0,t2,tg0,bg0},{v7,n0,t3,tg0,bg0},{v3,n0,t0,tg0,bg0},
                    {v7,n5,t0,tg2,bg2},{v6,n5,t1,tg2,bg2},{v5,n5,t2,tg2,bg2},{v5,n5,t2,tg2,bg2},{v4,n5,t3,tg2,bg2},{v7,n5,t0,tg2,bg2},
                    {v4,n1,t0,tg3,bg3},{v5,n1,t1,tg3,bg3},{v1,n1,t2,tg3,bg3},{v1,n1,t2,tg3,bg3},{v0,n1,t3,tg3,bg3},{v4,n1,t0,tg3,bg3},
                    {v4,n2,t0,tg4,bg4},{v0,n2,t1,tg4,bg4},{v3,n2,t2,tg4,bg4},{v3,n2,t2,tg4,bg4},{v7,n2,t3,tg4,bg4},{v4,n2,t0,tg4,bg4},
                    {v6,n3,t0,tg5,bg5},{v2,n3,t1,tg5,bg5},{v1,n3,t2,tg5,bg5},{v1,n3,t2,tg5,bg5},{v5,n3,t3,tg5,bg5},{v6,n3,t0,tg5,bg5}
            };

            RenderMesh* mesh = gRenderSystem->getRenderMeshManagerPtr()->createRenderMesh(name);
            mesh->setType(MeshType::MT_PNTBT);
            mesh->addGeometryInfo(data, data_count);
            mesh->addGeometryInfo(i, index_count);
            return mesh;
        }
    }

} // namespace Berserk