//
// Created by Egor Orachyov on 2019-08-08.
//

#ifndef BERSERK_RENDERINGTEST_H
#define BERSERK_RENDERINGTEST_H

#include <Rendering/RenderQueue.h>
#include <GLDriver.h>
#include <RenderScene.h>
#include <Rendering/VertexTypes.h>
#include <Rendering/MeshFactory.h>
#include <Engine/EngineUtils.h>

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

    static void RenderSceneTest()
    {
        IAllocator& allocator = Allocator::get();
        GlfwWindowManager manager(allocator);

        WindowRef resource = manager.createWindow(360, 360, "Test window");
        TSharedPtr<IWindow> window = resource.lock();

        window->makeActiveRenderingTarget();

        GLDriver driver(allocator);
        auto driverRef = EngineUtils::createPtr<RHIDriver>(driver);

        RenderScene scene;
        scene.setSceneName(String("Debug Scene"));
        scene.setDriver(driverRef);

        uint32 verticesCount = 4;
        VertPTf vertices[]
        {
                { Vec3f(-1, 1, 0),  Vec2f(0.0f, 1.0f)  },
                { Vec3f(-1, -1, 0), Vec2f(0.0f, 0.0f)  },
                { Vec3f(1, -1, 0),  Vec2f(1.0f, 0.0f)  },
                { Vec3f(1, 1, 0),  Vec2f(1.0f, 1.0f)  }
        };

        uint32 indicesCount = 6;
        uint16 indices[]
        {
                0, 1, 2, 2, 3, 0
        };

        MeshFactory factory(BU_DynamicDraw, IT_UnsignedShort, DL_VertexPT, PT_Triangles);
        factory.addMeshNode((uint8*) vertices, verticesCount,(uint8*) indices, indicesCount);
        TSharedPtr<Mesh> meshRef = factory.createMesh();
        auto mesh = EngineUtils::createHandle<Mesh>(meshRef);

        mesh->setName(String("Square_4p4i"));

        Renderable renderable;
        renderable.setMesh(mesh);
        auto renderableRef = EngineUtils::createPtr<Renderable>(renderable);

        StaticMeshComponent staticMesh;
        staticMesh.setRenderable(renderableRef);
        scene.addRenderable(staticMesh);

    }

    static void run()
    {
        //RenderQueueTest1();
        RenderSceneTest();
    }

};


#endif //BERSERK_RENDERINGTEST_H
