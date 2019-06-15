//
// Created by Egor Orachyov on 28.03.2019.
//

#ifndef BERSERK_RENDERINGSYSTEMTEST_H
#define BERSERK_RENDERINGSYSTEMTEST_H

#include <Helpers/MaterialManagerHelper.h>
#include <Foundation/RenderSystem.h>
#include <Memory/LinearAllocator.h>
#include "../../Engine/Source/CoreSystem/Utility/ProfilingUtility.h"
#include <FreeTypeImporter.h>
#include <Foundation/Font.h>
#include <Components/LightComponent.h>
#include <Components/SpotLightComponent.h>
#include <Components/PointLightComponent.h>
#include <Components/DirectionalLightComponent.h>

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

void ClassSizeInfo()
{
    using namespace Berserk;
    using namespace Berserk::Render;
    using namespace Berserk::Engine;
    using namespace Berserk::Resources;
    using namespace Berserk::Importers;

    printf("Type: %s size: %lu \n", IObject::getType(), sizeof(IObject));
    printf("Type: %s size: %lu \n", IEntity::getType(), sizeof(IEntity));
    printf("Type: %s size: %lu \n", IEntityComponent::getType(), sizeof(IEntityComponent));
    printf("Type: %s size: %lu \n", SceneComponent::getType(), sizeof(SceneComponent));
    printf("Type: %s size: %lu \n", IPrimitiveComponent::getType(), sizeof(IPrimitiveComponent));
    printf("Type: %s size: %lu \n", LightComponent::getType(), sizeof(LightComponent));
    printf("Type: %s size: %lu \n", DirectionalLightComponent::getType(), sizeof(DirectionalLightComponent));
    printf("Type: %s size: %lu \n", StaticMeshComponent::getType(), sizeof(StaticMeshComponent));
    printf("Type: %s size: %lu \n", CameraComponent::getType(), sizeof(CameraComponent));

    printf("IRenderSystem: %lu \n", sizeof(IRenderSystem));
    printf("RenderSystem: %lu \n", sizeof(RenderSystem));
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

    ClassSizeInfo();

    LinearAllocator allocator(Buffers::KiB * 128);
    IRenderSystem* render = new(allocator.allocate(sizeof(RenderSystem))) RenderSystem(ISystemInitializer("", &allocator));

    Timer timer;
    timer.start();

    while (!RenderBase::getMainWindow()->shouldClose())
    {
        timer.update();
        render->preUpdate();

        char frame[64]; auto fps = 1.0 / timer.elapsed();
        sprintf(frame, "Framerate: %3.1lf [ms: %5.4lf]", fps, timer.elapsed());
        RenderBase::getDebugRenderManager()->submit(Vec3f(10, 10 , 0), frame, DebugDrawManager::RED, 0.5);

        render->update();
        render->postUpdate();
    }

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

void FreeTypeImporterTest()
{
    using namespace Berserk;
    using namespace Berserk::Render;
    using namespace Berserk::Engine;
    using namespace Berserk::Resources;
    using namespace Berserk::Importers;

    FreeTypeImporter importer;
    LinearAllocator allocator(Buffers::KiB * 128);
    IRenderSystem* render = new(allocator.allocate(sizeof(RenderSystem))) RenderSystem(ISystemInitializer("", &allocator));

    // Font font("test", &allocator, nullptr);
    // font.mBitmap = RenderBase::getTextureManager()->createTexture("Bitmap");
    // importer.import("../Engine/Fonts/Arial.ttf", 16, &font);

    delete(render);
}

#endif //BERSERK_RENDERINGSYSTEMTEST_H
