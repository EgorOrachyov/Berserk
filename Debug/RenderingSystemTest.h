//
// Created by Egor Orachyov on 28.03.2019.
//

#ifndef BERSERK_RENDERINGSYSTEMTEST_H
#define BERSERK_RENDERINGSYSTEMTEST_H

#include "Helpers/MaterialManagerHelper.h"
#include <Foundation/RenderSystem.h>
#include <Memory/LinearAllocator.h>

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

    LinearAllocator allocator(Buffers::KiB * 256);

    IRenderSystem* render = new(allocator.allocate(sizeof(RenderSystem))) RenderSystem(ISystemInitializer("", &allocator));

    while (!RenderBase::getMainWindow()->shouldClose())
    {
        render->preUpdate();
        render->update();
        render->postUpdate();
    }

    delete (render);
    allocator.free(render);

    printf("Free calls: %u Alloc calls: %u Usage: %u Total mem: %lu \n",
           allocator.getFreeCalls(), allocator.getAllocateCalls(), allocator.getUsage(), allocator.getTotalMemoryUsage());
}

#endif //BERSERK_RENDERINGSYSTEMTEST_H
