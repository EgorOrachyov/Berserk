//
// Created by Egor Orachyov on 28.03.2019.
//

#ifndef BERSERK_RENDERINGSYSTEMTEST_H
#define BERSERK_RENDERINGSYSTEMTEST_H

#include "Helpers/MaterialManagerHelper.h"
#include <Foundation/RenderSystem.h>
#include <Memory/LinearAllocator.h>
#include <Profiling/ProfilingUtility.h>

void AllocatorInfo(Berserk::LinearAllocator& allocator)
{
    using namespace Berserk;

    char usage[Buffers::SIZE_64];
    char total[Buffers::SIZE_64];

    printf("Free calls: %u | Alloc calls: %u | Usage: %s | Total mem: %s \n",
           allocator.getFreeCalls(),
           allocator.getAllocateCalls(),
           ProfilingUtility::print(allocator.getUsage(), usage),
           ProfilingUtility::print((uint32)allocator.getTotalMemoryUsage(), total));
}

void MaterialImporterTest()
{
    using namespace Berserk::Resources;

    printf("Color: %s \n", MaterialManagerHelper::getColorRGBA("0.9001", "1.123", "4.0001", "-4.7").toString().get());
    printf("Type: %x \n", MaterialManagerHelper::getMaterialType("0101010101010101"));

}

void RenderSystemStartUp()
{
    using namespace Berserk;
    using namespace Berserk::Render;

    LinearAllocator allocator(Buffers::KiB * 128);

    IRenderSystem* render = new(allocator.allocate(sizeof(RenderSystem))) RenderSystem(ISystemInitializer("", &allocator));
    MeshComponent meshComponent;

    while (!RenderBase::getMainWindow()->shouldClose())
    {
        render->preUpdate();
        render->update();
        render->postUpdate();
    }

    printf("Total frames: %lu \n", render->getCurrentFrameNumber());

    delete (render);
    allocator.free(render);

    AllocatorInfo(allocator);
}

void RenderQueueTest()
{
    using namespace Berserk;
    using namespace Berserk::Render;
    using namespace Berserk::Engine;

    LinearAllocator allocator(Buffers::MiB);

    auto queue = new (allocator.allocate(sizeof(RenderQueue))) RenderQueue(&allocator);

    Timer timer;

    const uint32 COMPONENTS_COUNT = 500;
    StaticMeshComponent* components[COMPONENTS_COUNT];

    for (uint32 i = 0; i < COMPONENTS_COUNT; i++)
    {
        components[i] = IObject::createObject<StaticMeshComponent>(IObjectInitializer("Component", &allocator));
    }

    auto buffer = queue->getGeometryQueue();

    for (uint32 i = 0; i < COMPONENTS_COUNT; i++)
    {
        RenderQueueNode node;
        node.mRootComponent = components[i];
        node.mMeshComponent = MeshComponent((IGPUBuffer*)100, (IMaterial*)((uint32)Math::random(1.0f, 10.f)));
        node.mViewDepth = Math::random(0.0f, 100.0f);

        queue->submit(node);

        //printf("[%u] mat: %p depth: %f \n", i,
        //       buffer->get()[i].mMeshComponent.mRenderMaterial,
        //       buffer->get()[i].mViewDepth);
    }

    timer.start();
    buffer->sort();
    timer.update();
    printf("%lf time \n", timer.elapsed());

    for (uint32 i = 0; i < COMPONENTS_COUNT; i++)
    {
        printf("[%u] mat: %p depth: %f \n", i,
               buffer->get()[i].mMeshComponent.mRenderMaterial,
               buffer->get()[i].mViewDepth);
    }

    for (uint32 i = 0; i < COMPONENTS_COUNT; i++)
    {
        delete (components[i]);
    }

    delete (queue);
    allocator.free(queue);

    AllocatorInfo(allocator);
}

#endif //BERSERK_RENDERINGSYSTEMTEST_H
