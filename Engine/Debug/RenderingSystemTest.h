//
// Created by Egor Orachyov on 28.03.2019.
//

#ifndef BERSERK_RENDERINGSYSTEMTEST_H
#define BERSERK_RENDERINGSYSTEMTEST_H

#include "Helpers/MaterialManagerHelper.h"
#include <Foundation/RenderSystem.h>
#include <Memory/LinearAllocator.h>
#include <Profiling/ProfilingUtility.h>

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

    while (!RenderBase::getMainWindow()->shouldClose())
    {
        render->preUpdate();
        render->update();
        render->postUpdate();
    }

    printf("Total frames: %lu \n", render->getCurrentFrameNumber());

    delete (render);
    allocator.free(render);

    char usage[Buffers::SIZE_64];
    char total[Buffers::SIZE_64];

    printf("Free calls: %u | Alloc calls: %u | Usage: %s | Total mem: %s \n",
           allocator.getFreeCalls(),
           allocator.getAllocateCalls(),
           ProfilingUtility::print(allocator.getUsage(), usage),
           ProfilingUtility::print((uint32)allocator.getTotalMemoryUsage(), total));
}

#endif //BERSERK_RENDERINGSYSTEMTEST_H
