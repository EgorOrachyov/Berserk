//
// Created by Egor Orachyov on 2019-08-08.
//

#ifndef BERSERK_RENDERINGTEST_H
#define BERSERK_RENDERINGTEST_H

#include <Rendering/RenderQueue.h>

using namespace Berserk;

class RenderingTest
{
public:

    static void RenderQueueTest1()
    {
        RenderQueue queue;
        TArray<RenderElement> elements;

        elements.emplace(MeshHandle(), MeshNode(0,0,0,0), MaterialHandle(), Mat4x4f(), RHIGeometryBufferRef());

        for (uint32 i = 0; i < 50; i++)
        {
            queue.add(elements.get(0), Math::random(0.0f, 100.0f));
        }

        queue.sort();

        for (auto node = queue.getNodes().begin(); node != nullptr; node = queue.getNodes().next())
        {
            OutputDevice::printf("%p %u %f \n", &node->info, node->materialID, node->distanceFromView);
        }
    }

    static void run()
    {
        RenderQueueTest1();
    }

};


#endif //BERSERK_RENDERINGTEST_H
