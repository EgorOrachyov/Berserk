//
// Created by Egor Orachyov on 2019-07-18.
//

#ifndef BERSERK_GLRHITEST_H
#define BERSERK_GLRHITEST_H

#include <GLDriver.h>
#include <GlfwWindowManager.h>

using namespace Berserk;

class GLRHITest
{
public:

    static void StartUpTest1()
    {
        GlfwWindowManager manager;

        IWindowRef resource = manager.createWindow(360, 360, "Test window");
        TSharedPtr<IWindow> window = resource.lock();

        window->makeActiveRenderingTarget();

        GLDriver driver;

        uint32 verticesCount = 3;
        Vec3f vertices[]
        {
            Vec3f(0, 1, 0), Vec3f(-1, 0, 0), Vec3f(1, 0, 0)
        };

        uint32 indicesCount = 3;
        uint16 indices[]
        {
            0, 1, 2
        };

        RHIVertexBufferRef vertexBuffer = driver.createVertexBuffer(
                sizeof(Vec3f) * verticesCount, (uint8*) vertices, BU_StaticDraw);
        RHIIndexBufferRef indexBuffer = driver.createIndexBuffer(
                sizeof(uint16) * indicesCount, (uint8 *) indices, BU_StaticDraw, IT_UnsignedShort, indicesCount);
        RHIGeometryBufferRef geometry = driver.createGeometryBuffer(vertexBuffer, indexBuffer, DL_Vertex, PT_Triangles);

        char vertexShaderCode[] =
                              "#version 410 core\n"
                              "layout (location = 0) in vec3 VertexPosition;"
                              "void main()"
                              "{"
                              "gl_Position = vec4(VertexPosition, 1.0);"
                              "}";

        char fragmentShaderCode[] =
                                "#version 410 core\n"
                                "layout (location = 0) out vec4 FragColor;"
                                "void main()"
                                "{"
                                "FragColor = vec4(0.8f, 0.4f, 0.5f, 1.0f);"
                                "}";

        RHIVertexShaderRef vertexShader = driver.createVertexShader(vertexShaderCode);
        RHIFragmentShaderRef fragmentShader = driver.createFragmentShader(fragmentShaderCode);

        RHIShaderInitializer initializer;
        RHIShaderProgramRef program = driver.createShaderProgram(vertexShader, fragmentShader, initializer);

        driver.setFillMode(RFM_Solid);

        while (!window->shouldClose())
        {
            program->use();
            geometry->draw();
            window->swapBuffers();
            driver.swapBuffers();

            manager.update();
        }
    }

    static void run()
    {
        StartUpTest1();
    }

};


#endif //BERSERK_GLRHITEST_H
