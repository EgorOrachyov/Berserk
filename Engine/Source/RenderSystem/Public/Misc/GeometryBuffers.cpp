//
// Created by Egor Orachyov on 04.05.2019.
//

#include "GeometryBuffers.h"
#include <Platform/VertexTypes.h>

namespace Berserk::Render
{

    IGPUBuffer* GeometryBuffers::createBox(const char *name, Resources::IGPUBuffer::VertexType type, float32 width,
                                           float32 height, float32 depth)
    {
        IGPUBuffer* buffer = RenderBase::getBufferManager()->createGPUBuffer(name);

        if (buffer == nullptr)
        {
            return RenderBase::getBufferManager()->getGPUBuffer(name);
        }

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

        float32 x = width * 0.5f;
        float32 y = height * 0.5f;
        float32 z = depth * 0.5f;

        Vec3f v0 = Vec3f(-x,y,z),  v1 = Vec3f(-x,-y,z),
              v2 = Vec3f(x,-y,z),  v3 = Vec3f(x,y,z),
              v4 = Vec3f(-x,y,-z), v5 = Vec3f(-x,-y,-z),
              v6 = Vec3f(x,-y,-z), v7 = Vec3f(x,y,-z);

        Vec2f t0 = Vec2f(0,1), t1 = Vec2f(0,0),
              t2 = Vec2f(1,0), t3 = Vec2f(1.0,1.0);

        Vec3f n0 = Vec3f(1,0,0), n1 = Vec3f(-1,0,0),
              n2 = Vec3f(0,1,0), n3 = Vec3f(0,-1,0),
              n4 = Vec3f(0,0,1), n5 = Vec3f(0,0,-1);

        Vec3f tg0 = Vec3f(1,0,0),  tg1 = Vec3f(0,0,-1),
              tg2 = Vec3f(-1,0,0), tg3 = Vec3f(0,0,1),
              tg4 = Vec3f(1,0,0),  tg5 = Vec3f(-1,0,0);

        Vec3f bg0 = Vec3f(0,1,0),  bg1 = Vec3f(0,1,0),
              bg2 = Vec3f(0,1,0),  bg3 = Vec3f(0,1,0),
              bg4 = Vec3f(0,0,-1), bg5 = Vec3f(0,0,-1);

        const uint32 index_count = 36;
        uint16 index[index_count] =
        {
            0,1,2,3,4,5,
            6,7,8,9,10,11,
            12,13,14,15,16,17,
            18,19,20,21,22,23,
            24,25,26,27,28,29,
            30,31,32,33,34,35
        };

        if (type == IGPUBuffer::eVT_VertexPT)
        {
            const uint32 data_count = 36;
            VertPTf data[data_count] = {
                    {v0,t0},{v1,t1},{v2,t2},{v2,t2},{v3,t3},{v0,t0},
                    {v3,t0},{v2,t1},{v6,t2},{v6,t2},{v7,t3},{v3,t0},
                    {v7,t0},{v6,t1},{v5,t2},{v5,t2},{v4,t3},{v7,t0},
                    {v4,t0},{v5,t1},{v1,t2},{v1,t2},{v0,t3},{v4,t0},
                    {v4,t0},{v0,t1},{v3,t2},{v3,t2},{v7,t3},{v4,t0},
                    {v6,t0},{v2,t1},{v1,t2},{v1,t2},{v5,t3},{v6,t0}
            };

            buffer->create(data_count, type, data, index_count, index);
            return buffer;
        }

        if (type == IGPUBuffer::eVT_VertexPN)
        {
            const uint32 data_count = 36;
            VertPNf data[data_count] = {
                    {v0,n4},{v1,n4},{v2,n4},{v2,n4},{v3,n4},{v0,n4},
                    {v3,n0},{v2,n0},{v6,n0},{v6,n0},{v7,n0},{v3,n0},
                    {v7,n5},{v6,n5},{v5,n5},{v5,n5},{v4,n5},{v7,n5},
                    {v4,n1},{v5,n1},{v1,n1},{v1,n1},{v0,n1},{v4,n1},
                    {v4,n2},{v0,n2},{v3,n2},{v3,n2},{v7,n2},{v4,n2},
                    {v6,n3},{v2,n3},{v1,n3},{v1,n3},{v5,n3},{v6,n3}
            };

            buffer->create(data_count, type, data, index_count, index);
            return buffer;
        }

        if (type == IGPUBuffer::eVT_VertexPNT)
        {
            const uint32 data_count = 36;
            VertPNTf data[data_count] = {
                    {v0,n4,t0},{v1,n4,t1},{v2,n4,t2},{v2,n4,t2},{v3,n4,t3},{v0,n4,t0},
                    {v3,n0,t0},{v2,n0,t1},{v6,n0,t2},{v6,n0,t2},{v7,n0,t3},{v3,n0,t0},
                    {v7,n5,t0},{v6,n5,t1},{v5,n5,t2},{v5,n5,t2},{v4,n5,t3},{v7,n5,t0},
                    {v4,n1,t0},{v5,n1,t1},{v1,n1,t2},{v1,n1,t2},{v0,n1,t3},{v4,n1,t0},
                    {v4,n2,t0},{v0,n2,t1},{v3,n2,t2},{v3,n2,t2},{v7,n2,t3},{v4,n2,t0},
                    {v6,n3,t0},{v2,n3,t1},{v1,n3,t2},{v1,n3,t2},{v5,n3,t3},{v6,n3,t0}
            };

            buffer->create(data_count, type, data, index_count, index);
            return buffer;
        }

        if (type == IGPUBuffer::eVT_VertexPNTBT)
        {
            const uint32 data_count = 36;
            VertPNTBTf data[data_count] =
            {
                {v0,n4,tg0,bg0,t0},{v1,n4,tg0,bg0,t1},{v2,n4,tg0,bg0,t2},{v2,n4,tg0,bg0,t2},{v3,n4,tg0,bg0,t3},{v0,n4,tg0,bg0,t0},
                {v3,n0,tg1,bg1,t0},{v2,n0,tg1,bg1,t1},{v6,n0,tg1,bg1,t2},{v6,n0,tg1,bg1,t2},{v7,n0,tg1,bg1,t3},{v3,n0,tg1,bg1,t0},
                {v7,n5,tg2,bg2,t0},{v6,n5,tg2,bg2,t1},{v5,n5,tg2,bg2,t2},{v5,n5,tg2,bg2,t2},{v4,n5,tg2,bg2,t3},{v7,n5,tg2,bg2,t0},
                {v4,n1,tg3,bg3,t0},{v5,n1,tg3,bg3,t1},{v1,n1,tg3,bg3,t2},{v1,n1,tg3,bg3,t2},{v0,n1,tg3,bg3,t3},{v4,n1,tg3,bg3,t0},
                {v4,n2,tg4,bg4,t0},{v0,n2,tg4,bg4,t1},{v3,n2,tg4,bg4,t2},{v3,n2,tg4,bg4,t2},{v7,n2,tg4,bg4,t3},{v4,n2,tg4,bg4,t0},
                {v6,n3,tg5,bg5,t0},{v2,n3,tg5,bg5,t1},{v1,n3,tg5,bg5,t2},{v1,n3,tg5,bg5,t2},{v5,n3,tg5,bg5,t3},{v6,n3,tg5,bg5,t0}
            };

            buffer->create(data_count, type, data, index_count, index);
            return buffer;
        }
    }

    IGPUBuffer* GeometryBuffers::createCube(const char *name, Resources::IGPUBuffer::VertexType type, float32 size)
    {
        return GeometryBuffers::createBox(name, type, size, size, size);
    }

    IGPUBuffer* GeometryBuffers::createPlane(const char *name, Resources::IGPUBuffer::VertexType type, float32 width, float32 depth)
    {
        IGPUBuffer* buffer = RenderBase::getBufferManager()->createGPUBuffer(name);

        if (buffer == nullptr)
        {
            return RenderBase::getBufferManager()->getGPUBuffer(name);
        }

        //      v0 ------ v3
        //     /          /
        //    /          /
        //   /          /
        //  /          /
        // v1 ------ v2

        float32 x = width * 0.5f;
        float32 z = depth * 0.5f;

        Vec3f v0 = Vec3f(-x,0,-z),  v1 = Vec3f(-x,0,z),
              v2 = Vec3f(x,0,z),  v3 = Vec3f(x,0,-z);

        Vec2f t0 = Vec2f(0,1), t1 = Vec2f(0,0),
              t2 = Vec2f(1,0), t3 = Vec2f(1,1);

        Vec3f n = Vec3f(0,1,0);

        Vec3f tg = Vec3f(1,0,0);

        Vec3f bg = Vec3f(0,0,1);

        const uint32 index_count = 6;
        uint16 index[index_count] =
        {
            0,1,2,2,3,0
        };

        if (type == IGPUBuffer::eVT_VertexPT)
        {
            const uint32 data_count = 4;
            VertPTf data[data_count] =
            {
                {v0,t0},{v1,t1},{v2,t2},{v3,t3}
            };

            buffer->create(data_count, type, data, index_count, index);
            return buffer;
        }

        if (type == IGPUBuffer::eVT_VertexPN)
        {
            const uint32 data_count = 4;
            VertPNf data[data_count] =
            {
                {v0,n},{v1,n},{v2,n},{v3,n}
            };

            buffer->create(data_count, type, data, index_count, index);
            return buffer;
        }

        if (type == IGPUBuffer::eVT_VertexPNT)
        {
            const uint32 data_count = 4;
            VertPNTf data[data_count] =
            {
                {v0,n,t0},{v1,n,t1},{v2,n,t2},{v3,n,t3}
            };

            buffer->create(data_count, type, data, index_count, index);
            return buffer;
        }

        if (type == IGPUBuffer::eVT_VertexPNTBT)
        {
            const uint32 data_count = 4;
            VertPNTBTf data[data_count] =
            {
                {v0,n,tg,bg,t0},{v1,n,tg,bg,t1},{v2,n,tg,bg,t2},{v3,n,tg,bg,t3}
            };

            buffer->create(data_count, type, data, index_count, index);
            return buffer;
        }
    }

    IGPUBuffer* GeometryBuffers::createScreen(const char *name, float32 width, float32 height)
    {
        IGPUBuffer* buffer = RenderBase::getBufferManager()->createGPUBuffer(name);

        if (buffer == nullptr)
        {
            return RenderBase::getBufferManager()->getGPUBuffer(name);
        }

        float32 w = width * 0.5f;
        float32 h = height * 0.5f;

        Vec3f v0(-w, -h, 0), v1(w, -h, 0), v2(w, h, 0),   v3(-w, h, 0);
        Vec2f t0 = Vec2f(0,0), t1 = Vec2f(1,0), t2 = Vec2f(1,1), t3 = Vec2f(0,1);

        const uint32 data_count = 4;
        VertPTf data[data_count] = { {v0,t0}, {v1,t1}, {v2,t2}, {v3,t3} };

        const uint32 index_count = 6;
        uint16 index[index_count] = { 0, 1, 2, 2, 3, 0 };

        buffer->create(data_count, IGPUBuffer::eVT_VertexPT, data, index_count, index);

        return buffer;
    }

} // namespace Berserk::Render