//
// Created by Egor Orachyov on 2019-07-18.
//

#ifndef BERSERK_GLRHITEST_H
#define BERSERK_GLRHITEST_H

#include <GLDriver.h>
#include <GlfwWindowManager.h>
#include <Rendering/VertexTypes.h>

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
        VertPTf vertices[]
        {
            { Vec3f(0, 1, 0),  Vec2f(0.5f, 1.0f)  },
            { Vec3f(-1, 0, 0), Vec2f(0.0f, 0.0f)  },
            { Vec3f(1, 0, 0),  Vec2f(1.0f, 0.0f)  }
        };

        uint32 indicesCount = 3;
        uint16 indices[]
        {
            0, 1, 2
        };

        uint32 width = 2;
        uint32 height = 2;
        uint32 textureData[]
        {
                0x00ff0000, 0x00ff0000,
                0x00ff0000, 0x00ff0000
        };

        RHITexture2DRef texture2D = driver.createTexture(
                width,
                height,
                SF_RGBA8,
                PF_BGRA,
                DT_UnsignedByte,
                (uint8*) textureData,
                false);

        RHISamplerRef sampler = driver.createSampler(
                SF_Linear,
                SF_Linear,
                SWM_ClampToBorder);

        RHIVertexBufferRef vertexBuffer = driver.createVertexBuffer(
                sizeof(vertices),
                (uint8*) vertices,
                BU_StaticDraw);

        RHIIndexBufferRef indexBuffer = driver.createIndexBuffer(
                sizeof(uint16) * indicesCount,
                (uint8 *) indices,
                BU_StaticDraw,
                IT_UnsignedShort,
                indicesCount);

        RHIGeometryBufferRef geometry = driver.createGeometryBuffer(
                vertexBuffer,
                indexBuffer,
                DL_VertexPT,
                PT_Triangles);

        char vertexShaderCode[] =
                              "#version 410 core\n"
                              "layout (location = 0) in vec3 VertexPosition;"
                              "layout (location = 1) in vec2 VertexTexCoords;"
                              "out vec2 FragTexCoords;"
                              "layout (std140) uniform Transform"
                              "{"
                              "    mat4 Model;"
                              "};"
                              "void main()"
                              "{"
                              "FragTexCoords = VertexTexCoords;"
                              "gl_Position = Model * vec4(VertexPosition, 1.0);"
                              "}";

        char fragmentShaderCode[] =
                              "#version 410 core\n"
                              "layout (location = 0) out vec4 FragColor;"
                              "in vec2 FragTexCoords;"
                              "uniform sampler2D Texture0;"
                              "void main()"
                              "{"
                              "vec3 color = texture(Texture0, FragTexCoords).rgb;"
                              "FragColor = vec4(color, 1.0f);"
                              "}";


        RHIVertexShaderRef vertexShader = driver.createVertexShader(vertexShaderCode);
        RHIFragmentShaderRef fragmentShader = driver.createFragmentShader(fragmentShaderCode);

        RHIShaderInitializer initializer;
        initializer.uniformVarNames.emplace("Texture0");
        initializer.uniformBlocksInfo.emplace(String("Transform"), 0);

        RHIShaderProgramRef program = driver.createShaderProgram(vertexShader, fragmentShader, initializer);
        RHIUniformBufferRef uniformBuffer = driver.createUniformBuffer(0, sizeof(Mat4x4f), nullptr, BU_DynamicDraw);

        driver.setFillMode(RFM_Solid);
        driver.setClearColor(Vec4f());
        while (!window->shouldClose())
        {
            static float32 angle = 0.0f;
            angle += 0.01;
            Mat4x4f t = Mat4x4f::rotateX(angle);

            program->use();
            program->setUniform("Texture0", 0u);
            uniformBuffer->update(sizeof(Mat4x4f), (const uint8*) &t);
            uniformBuffer->bind();
            texture2D->bind(0, sampler);
            geometry->draw();
            window->swapBuffers();
            driver.swapBuffers();
            driver.clearColorBuffer();

            manager.update();
        }
    }

    static void run()
    {
        StartUpTest1();
    }

};


#endif //BERSERK_GLRHITEST_H
