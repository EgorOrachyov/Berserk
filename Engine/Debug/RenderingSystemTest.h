//
// Created by Egor Orachyov on 28.03.2019.
//

#ifndef BERSERK_RENDERINGSYSTEMTEST_H
#define BERSERK_RENDERINGSYSTEMTEST_H

#include <Helpers/MaterialManagerHelper.h>
#include <Foundation/RenderSystem.h>
#include <Memory/LinearAllocator.h>
#include <Profiling/ProfilingUtility.h>
#include <FreeTypeImporter.h>
#include <Foundation/Font.h>
#include <Components/LightSourceComponent.h>
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
    printf("Type: %s size: %lu \n", LightSourceComponent::getType(), sizeof(LightSourceComponent));
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

    Vec3f v0(-1, -1, 0), v1(1, -1, 0),
            v2(1, 1, 0),   v3(-1, 1, 0);

    Vec2f t0 = Vec2f(0,0), t1 = Vec2f(1,0),
            t2 = Vec2f(1,1), t3 = Vec2f(0,1);

    IFont* font = RenderBase::getFontManager()->getDebugFont();

    // /*

    const uint32 data_count = 4;
    VertPTf data[data_count] =
    {
            {v0,font->getCharacters()->get()['B'].texturePos},
            {v1,font->getCharacters()->get()['B'].texturePos + Vec2f(font->getCharacters()->get()['B'].textureSize.x, 0)},
            {v2,font->getCharacters()->get()['B'].texturePos + font->getCharacters()->get()['B'].textureSize},
            {v3,font->getCharacters()->get()['B'].texturePos + Vec2f(0, font->getCharacters()->get()['B'].textureSize.y)}
    };

    // */

    /*

    const uint32 data_count = 4;
    VertPTf data[data_count] =
    {
            {v0,t0}, {v1,t1}, {v2,t2}, {v3,t3}
    };

    */

    const uint32 index_count = 6;
    uint16 i[index_count]
    {
            0, 1, 2, 2, 3, 0
    };

    IGPUBuffer* screen = RenderBase::getBufferManager()->createGPUBuffer("ScreenPlane");
    screen->create(data_count, IGPUBuffer::eVT_VertexPT, data, index_count, i);

    IRenderDriver* driver = RenderBase::getRenderDriver();
    IWindow* window = RenderBase::getMainWindow();
    IShader* screenRender = RenderBase::getShaderManager()->loadShader("{SHADERS}/Debug/ScreenRender/meta-info.xml");

    Timer timer;
    timer.start();

    while (!RenderBase::getMainWindow()->shouldClose())
    {
        timer.update();

        render->preUpdate();

        // uint32 width, height;
        // window->getFrameBufferSize(width, height);
        // auto displayBufferVP = IRenderDriver::ViewPort(0, 0, width, height);

        // screenRender->use();
        // screenRender->setUniform("Texture0", 0);
        // screenRender->setUniform("ViewPort", Vec4f(displayBufferVP.x, displayBufferVP.y, displayBufferVP.width, displayBufferVP.height));
        // font->getTexture()->bind(0);
        // driver->bindDefaultFrameBuffer();
        // driver->clear(true, true, false);
        // driver->depthTest(false);
        // driver->viewPort(displayBufferVP);
        // screen->draw();

        char message1[] = "Hello, world!";
        char message2[] = "It is debug draw manager [text rendering]";
        char frame[64]; auto fps = 1.0 / timer.elapsed();
        sprintf(frame, "Framerate: %2.3lf", fps);
        RenderBase::getDebugRenderManager()->submit(Vec3f(10, 70 , 0), frame,    DebugDrawManager::RED, 0.5);
        RenderBase::getDebugRenderManager()->submit(Vec3f(10, 40 , 0), message1, DebugDrawManager::WHITE, 0.5);
        RenderBase::getDebugRenderManager()->submit(Vec3f(10, 10 , 0), message2, DebugDrawManager::YELLOW, 0.5);

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
