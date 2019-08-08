//
// Created by Egor Orachyov on 2019-07-18.
//

#ifndef BERSERK_GLRHITEST_H
#define BERSERK_GLRHITEST_H

#include <GLDriver.h>
#include <GlfwWindowManager.h>
#include <Rendering/VertexTypes.h>
#include <FreeImageImporter.h>
#include <Importers/XMLShaderImporter.h>
#include <FreeTypeImporter.h>
#include <Rendering/MeshFactory.h>

using namespace Berserk;

class GLRHITest
{
public:

    static void StartUpTest1()
    {
        XMLShaderImporter shaderImporter;
        auto importOptions = shaderImporter.import("../Engine/Shaders/Debug/Default/meta-info.xml");

        IAllocator& allocator = Allocator::get();
        GlfwWindowManager manager(allocator);

        IWindowRef resource = manager.createWindow(360, 360, "Test window");
        TSharedPtr<IWindow> window = resource.lock();

        window->makeActiveRenderingTarget();

        GLDriver driver(allocator);
        FreeImageImporter imageImporter(allocator);

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

        uint32 width = 2;
        uint32 height = 2;
        char textureName[] = "texture.jpg";
        auto imageData = imageImporter.load(textureName);

        RHITexture2DRef texture2D = driver.createTexture(
                imageData->getWidth(),
                imageData->getHeight(),
                imageData->getStorageFormat(),
                imageData->getPixelFormat(),
                imageData->getDataType(),
                imageData->getBuffer(),
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
                IT_UnsignedShort);

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

        ShaderInitializer initializer(allocator);
        initializer.uniformVarNames.emplace("Texture0");
        initializer.uniformBlocksInfo.emplace(String("Transform"), 0);

        RHIShaderProgramRef program = driver.createShaderProgram(vertexShader, fragmentShader, initializer);
        RHIUniformBufferRef uniformBuffer = driver.createUniformBuffer(0, sizeof(Mat4x4f), nullptr, BU_DynamicDraw);

        RHIFrameBufferRef frameBuffer = driver.createFrameBuffer(360, 360, SF_RGB16F);

        RHIFaceCullingStateRef faceCullingState = driver.createFaceCullingState(FC_Back, RCM_CounterClockwise);
        RHIDepthTestStateRef depthTestState = driver.createDepthState(true, CF_LessEqual);
        RHIBlendStateRef blendState = driver.createBlendState(BF_SrcAlpha, BF_OneMinusSrcAlpha);

        driver.setFillMode(RFM_Solid);
        driver.setClearColor(Vec4f());

        {
            static float32 angle = 0.33f;
            angle += 0.01;
            Mat4x4f t = Mat4x4f::rotateZ(angle);

            frameBuffer->bind();
            driver.clearColorBuffer();
            driver.setViewport(ViewPort(0,0,360,360));

            program->use();
            program->setUniform("Texture0", 0u);
            uniformBuffer->update(sizeof(Mat4x4f), (const uint8*) &t);
            uniformBuffer->bind();
            texture2D->bind(0, sampler);
            geometry->draw(6, 0);

            manager.update();
        }

        RHITextureRef texture = frameBuffer->getColorAttachment(0);

        faceCullingState->enable();
        depthTestState->enable();
        blendState->enable();

        while (!window->shouldClose())
        {
            static float32 angle = 0.0f;
            angle += 0.01;
            Mat4x4f t = Mat4x4f::rotateX(angle);

            driver.bindDefaultFrameBuffer();
            driver.clearColorBuffer();
            driver.clearDepthBuffer();
            driver.setViewport(ViewPort(0,0,360 * 2,360 * 2));

            program->use();
            program->setUniform("Texture0", 0u);
            uniformBuffer->update(sizeof(Mat4x4f), (const uint8*) &t);
            uniformBuffer->bind();
            texture->bind(0, sampler);
            geometry->draw(6, 0);
            window->swapBuffers();

            driver.swapBuffers();
            manager.update();
        }

        uint32 size = 3 * texture2D->getWidth() * texture2D->getHeight();
        ImageImportData data(texture2D->getWidth(), texture2D->getHeight(), DT_UnsignedByte, PF_RGB, SF_RGBA8, size);
        texture2D->readData(PF_RGB, DT_UnsignedByte, data.getBuffer());

        imageImporter.save("save.bmp", data);

        ShaderImportData::output(importOptions.get(), OutputDevice::get());
    }

    static void FontTest1()
    {
        IAllocator& allocator = Allocator::get();
        GlfwWindowManager manager(allocator);

        IWindowRef resource = manager.createWindow(360, 360, "Test window");
        TSharedPtr<IWindow> window = resource.lock();

        window->makeActiveRenderingTarget();

        GLDriver driver(allocator);
        FreeImageImporter imageImporter(allocator);

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

        char buffer[20];

        MeshFactory factory(BU_DynamicDraw, IT_UnsignedShort, DL_VertexPT, PT_Triangles);
        factory.addMeshNode((uint8*) vertices, verticesCount,(uint8*) indices, indicesCount);
        TSharedPtr<Mesh> mesh = factory.createMesh();
        auto data = new (buffer) ResourceHandleData((TSharedPtr<IResource>) mesh);
        MeshRef meshRef(TSharedPtr<ResourceHandleData>(data, nullptr));
        meshRef->setName(String("Square_4p4i"));

        RHISamplerRef sampler = driver.createSampler(
                SF_Linear,
                SF_Linear,
                SWM_ClamptToEdge);

        RHIVertexBufferRef vertexBuffer = driver.createVertexBuffer(
                meshRef->getVerticesBuffer().getSize(),
                meshRef->getVerticesBuffer().getRawBuffer(),
                meshRef->getVertexBufferUsage());

        RHIIndexBufferRef indexBuffer = driver.createIndexBuffer(
                meshRef->getIndicesBuffer().getSize(),
                meshRef->getIndicesBuffer().getRawBuffer(),
                meshRef->getIndexBufferUsage(),
                meshRef->getIndicesType());

        RHIGeometryBufferRef geometry = driver.createGeometryBuffer(
                vertexBuffer,
                indexBuffer,
                meshRef->getVerticesType(),
                meshRef->getPrimitiveType());

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
                "float alpha = texture(Texture0, FragTexCoords).r;"
                "FragColor = vec4(vec3(1,0,0), alpha);"
                "}";

        RHIVertexShaderRef vertexShader = driver.createVertexShader(vertexShaderCode);
        RHIFragmentShaderRef fragmentShader = driver.createFragmentShader(fragmentShaderCode);

        ShaderInitializer initializer(allocator);
        initializer.uniformVarNames.emplace("Texture0");
        initializer.uniformBlocksInfo.emplace(String("Transform"), 0);

        RHIShaderProgramRef program = driver.createShaderProgram(vertexShader, fragmentShader, initializer);
        RHIUniformBufferRef uniformBuffer = driver.createUniformBuffer(0, sizeof(Mat4x4f), nullptr, BU_DynamicDraw);

        RHIFrameBufferRef frameBuffer = driver.createFrameBuffer(360, 360, SF_RGB16F);

        RHIFaceCullingStateRef faceCullingState = driver.createFaceCullingState(FC_Back, RCM_CounterClockwise);
        RHIDepthTestStateRef depthTestState = driver.createDepthState(true, CF_LessEqual);
        RHIBlendStateRef blendState = driver.createBlendState(BF_SrcAlpha, BF_OneMinusSrcAlpha);

        FreeTypeImporter fontImporter(allocator);
        auto font = fontImporter.load("../Resources/Fonts/Arial.ttf", "Arial", 40);
        auto bitmap = font->getBitmap();
        auto texture = driver.createTexture(
                bitmap->getWidth(),
                bitmap->getHeight(),
                bitmap->getStorageFormat(),
                bitmap->getPixelFormat(),
                bitmap->getDataType(),
                bitmap->getBuffer(),
                false);

        driver.setFillMode(RFM_Solid);
        driver.setClearColor(Vec4f());

        blendState->enable();

        while (!window->shouldClose())
        {
            Mat4x4f t;

            driver.bindDefaultFrameBuffer();
            driver.clearColorBuffer();
            driver.clearDepthBuffer();

            CharData& data = font->getCharData().get('@');
            Vec4f texCoords = data.texturePos;
            VertPTf glyph[] = {
                    { Vec3f(-1, 1, 0),  Vec2f(texCoords.x, texCoords.w)  },
                    { Vec3f(-1, -1, 0), Vec2f(texCoords.x, texCoords.y)  },
                    { Vec3f(1, -1, 0),  Vec2f(texCoords.z, texCoords.y)  },
                    { Vec3f(1, 1, 0),   Vec2f(texCoords.z, texCoords.w)  }
            };

            vertexBuffer->update(sizeof(glyph), (uint8*) glyph);

            program->use();
            program->setUniform("Texture0", 0u);
            uniformBuffer->update(sizeof(Mat4x4f), (const uint8*) &t);
            uniformBuffer->bind();
            texture->bind(0, sampler);
            geometry->draw(mesh->getMeshNodes().get(0).getIndicesCount(), mesh->getMeshNodes().get(0).getIndicesOffset());
            window->swapBuffers();

            driver.swapBuffers();
            manager.update();
        }

        OutputDevice::printf("%s \n", driver.getDriverName().get());
        OutputDevice::printf("%s \n", driver.getVendorName().get());
        OutputDevice::printf("%s \n", driver.getShadingLanguageName().get());
    }

    static void run()
    {
        // StartUpTest1();
        FontTest1();
    }

};


#endif //BERSERK_GLRHITEST_H
