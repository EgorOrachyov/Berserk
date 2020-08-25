/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>

#include <Main.h>
#include <Engine.h>
#include <RHI/RHIDevice.h>
#include <Platform/Input.h>
#include <Platform/WindowManager.h>
#include <Platform/System.h>
#include <RenderTargets/WindowTarget.h>
#include <Paths.h>
#include <ResourceImporters.h>
#include <Math/Vec2f.h>
#include <Math/Vec3f.h>
#include <Resources/Mesh.h>
#include <Resources/MeshImportOptions.h>
#include <Math/Mat4x4f.h>
#include <RenderModule.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestRenderMesh)
{
    BERSERK_TEST_COND(RenderMesh, true)
    {
        Main main;
        main.initialize(0, nullptr);
        {
            // References to common engine singletons
            auto& system = System::getSingleton();
            auto& engine = Engine::getSingleton();
            auto& input = Input::getSingleton();
            auto& device = RHIDevice::getSingleton();
            auto& windowManager = WindowManager::getSingleton();
            auto& importers = ResourceImporters::getSingleton();
            auto& renderer = Render::RenderModule::getSingleton();

            // Default window, created on engine start-up
            auto window = windowManager.find("MAIN_WINDOW");

            MeshImportOptions options;
            TPtrShared<Mesh> mesh;
            options.setFormat({EMeshAttribute::Position, EMeshAttribute::Normal, EMeshAttribute::Tangent, EMeshAttribute::TexCoord });
            options.setTransform(Transformf().scale(Vec3f(8.0f)));
            auto path = Paths::getFullPathFor("Assets/Models/sphere.obj", EPathType::Engine);
            auto importer = importers.findImporterFromPath(path);

            if (importer.isNotNull()) {
                TPtrShared<Resource> res;
                importer->import(res, path, options);
                mesh = res.castTo<Mesh>();
            }

            auto positionBuffer = device.createVertexBuffer(mesh->getVertexData().size(), EBufferUsage::Static, mesh->getVertexData().data());
            auto indexBuffer = device.createIndexBuffer(mesh->getIndexData().size(), EBufferUsage::Static, mesh->getIndexData().data());

            RHIVertexDeclarationDesc vertexDeclarationDesc;
            {
                uint32 stride = mesh->getStride();

                vertexDeclarationDesc.resize(4);
                vertexDeclarationDesc[0].buffer = 0;
                vertexDeclarationDesc[0].iterating = EVertexIterating::PerVertex;
                vertexDeclarationDesc[0].location = 0;
                vertexDeclarationDesc[0].offset = 0;
                vertexDeclarationDesc[0].stride = stride;
                vertexDeclarationDesc[0].type = EVertexElementType::Float3;
                vertexDeclarationDesc[1].buffer = 0;
                vertexDeclarationDesc[1].iterating = EVertexIterating::PerVertex;
                vertexDeclarationDesc[1].location = 1;
                vertexDeclarationDesc[1].offset = vertexDeclarationDesc[0].offset + sizeof(Vec3f);
                vertexDeclarationDesc[1].stride = stride;
                vertexDeclarationDesc[1].type = EVertexElementType::Float3;
                vertexDeclarationDesc[2].buffer = 0;
                vertexDeclarationDesc[2].iterating = EVertexIterating::PerVertex;
                vertexDeclarationDesc[2].location = 2;
                vertexDeclarationDesc[2].offset = vertexDeclarationDesc[1].offset + sizeof(Vec3f);
                vertexDeclarationDesc[2].stride = stride;
                vertexDeclarationDesc[2].type = EVertexElementType::Float3;
                vertexDeclarationDesc[3].buffer = 0;
                vertexDeclarationDesc[3].iterating = EVertexIterating::PerVertex;
                vertexDeclarationDesc[3].location = 3;
                vertexDeclarationDesc[3].offset = vertexDeclarationDesc[2].offset + sizeof(Vec3f);
                vertexDeclarationDesc[3].stride = stride;
                vertexDeclarationDesc[3].type = EVertexElementType::Float2;
            }

            auto declaration = device.createVertexDeclaration(vertexDeclarationDesc);
            auto array = device.createArrayObject({ positionBuffer }, indexBuffer, declaration, mesh->getPrimitivesType());

            // Vertex shader code in GLSL
            char vertexShader[] =
                    "#version 410 core\n"
                    "layout (location = 0) in vec3 position;"
                    "layout (location = 1) in vec3 normal;"
                    "layout (location = 2) in vec3 tangent;"
                    "layout (location = 3) in vec2 texCoords;"
                    ""
                    "out vec3 fsColor;"
                    "out vec3 fsPos;"
                    "out vec3 fsNorm;"
                    "out vec3 fsTang;"
                    "out vec3 fsBitang;"
                    "out vec2 fsTexCoords;"
                    "flat out vec3 fsLightPos;"
                    ""
                    "layout (std140) uniform Transform {"
                    "  mat4 proj;"
                    "  mat4 view;"
                    "  mat4 model;"
                    "  mat4 norm;"
                    "  vec3 lightPos;"
                    "};"
                    ""
                    "void main() {"
                    " fsPos = (model * vec4(position, 1.0f)).xyz;"
                    " fsNorm = normalize((norm * vec4(normal, 0.0f)).xyz);"
                    " fsTang = normalize((norm * vec4(tangent, 0.0f)).xyz);"
                    " fsBitang = cross(fsNorm,fsTang);"
                    " fsColor = vec3(0.5f) + vec3(0.5f) * fsNorm;"
                    " fsTexCoords = texCoords;"
                    " fsLightPos = lightPos;"
                    " gl_Position = proj * view * vec4(fsPos, 1.0f);"
                    "}";

            // Fragment shader code in GLSL
            char fragmentShader[] =
                    "#version 410 core\n"
                    "layout (location = 0) out vec4 outColor;"
                    ""
                    "in vec3 fsColor;"
                    "in vec3 fsPos;"
                    "in vec3 fsNorm;"
                    "in vec3 fsTang;"
                    "in vec3 fsBitang;"
                    "in vec2 fsTexCoords;"
                    "flat in vec3 fsLightPos;"
                    ""
                    "uniform sampler2D Diffuse;"
                    "uniform sampler2D Normal;"
                    ""
                    "vec3 getNormal() {"
                    "  vec3 T = normalize(fsTang);"
                    "  vec3 B = normalize(fsBitang);"
                    "  vec3 N = normalize(fsNorm);"
                    "  mat3 TBN = mat3(T,B,N);"
                    "  vec3 normal;"
                    "  normal = texture(Normal, fsTexCoords).xyz;"
                    "  normal = normalize(normal * 2.0f - 1.0f);"
                    "  normal = normalize(TBN * normal);"
                    "  return normal;"
                    "}"
                    ""
                    "vec3 getDiffuse() {"
                    "  vec3 color;"
                    "  color = texture(Diffuse, fsTexCoords).rgb;"
                    "  color = pow(color, vec3(2.2f));"
                    "  return color;"
                    "}"
                    ""
                    "vec3 light() {"
                    "  vec3 Lp = fsLightPos;"
                    "  vec3 Lc = vec3(0.99f);"
                    "  vec3 D = normalize(Lp - fsPos);"
                    "  vec3 N = getNormal();"
                    "  float NdotL = max(dot(N,D), 0.0f);"
                    "  return vec3(NdotL) * Lc  + vec3(0.1,0.1,0.1);"
                    "}"
                    ""
                    "void main() {"
                    " vec3 color = light() * getDiffuse();"
                    " vec3 gamma = pow(color,vec3(1.0f/2.2f));"
                    " outColor = vec4(gamma, 1.0f);"
                    "}";

            RHIShaderDesc shaderDesc;
            {
                shaderDesc.resize(2);
                shaderDesc[0].type = EShaderType::Vertex;
                shaderDesc[0].code.add((uint8*) vertexShader, CStringUtility::length(vertexShader));
                shaderDesc[1].type = EShaderType::Fragment;
                shaderDesc[1].code.add((uint8*) fragmentShader, CStringUtility::length(fragmentShader));
            }

            auto shader = device.createShader(EShaderLanguage::GLSL, shaderDesc);
            auto meta = device.createShaderMeta(shader);

            RHIGraphicsPipelineState pipelineState;
            {
                pipelineState.polygonMode = EPolygonMode::Fill;
                pipelineState.polygonFrontFace = EPolygonFrontFace::CounterClockwise;
                pipelineState.polygonCullMode = EPolygonCullMode::Back;
                pipelineState.depthTest = true;
                pipelineState.depthWrite = true;
                pipelineState.depthCompare = ECompareFunction::Less;
                pipelineState.shader = shader;
                pipelineState.vertexDeclaration = declaration;
            }

            auto pTransform = meta->getUniformBlock("Transform");
            auto pProj = pTransform->getMember("proj");
            auto pView = pTransform->getMember("view");
            auto pModel = pTransform->getMember("model");
            auto pNorm = pTransform->getMember("norm");
            auto pLightPos = pTransform->getMember("lightPos");
            auto pDiffuse = meta->getParam("Diffuse");
            auto pNormal = meta->getParam("Normal");

            auto uniformBuffer = device.createUniformBuffer(pTransform->getSize(), EBufferUsage::Dynamic, nullptr);

            auto imageDiffuse = Image::loadImage(*Image::getDefaultImportOptions(), Paths::getFullPathFor("Assets/Textures/wall_diffuse.jpg", EPathType::Engine));
            auto textureDiffuse = device.createTexture2D(EBufferUsage::Static, true, *imageDiffuse);

            auto imageNormal = Image::loadImage(*Image::getDefaultImportOptions(), Paths::getFullPathFor("Assets/Textures/wall_normal.jpg", EPathType::Engine));
            auto textureNormal = device.createTexture2D(EBufferUsage::Static, true, *imageNormal);

            RHISamplerDesc samplerDesc;
            {
                samplerDesc.min = ESamplerFilter::Linear;
                samplerDesc.mag = ESamplerFilter::Linear;
                samplerDesc.mipmapMode = ESamplerFilter::Linear;
                samplerDesc.useMips = true;
                samplerDesc.u = ESamplerRepeatMode::ClampToEdge;
                samplerDesc.v = ESamplerRepeatMode::ClampToEdge;
            }

            auto sampler = device.createSampler(samplerDesc);

            RHIUniformBlockDesc blockDesc;
            {
                blockDesc.binding = pTransform->getBinding();
                blockDesc.buffer = uniformBuffer;
                blockDesc.offset = 0;
                blockDesc.range = pTransform->getSize();
            }

            RHIUniformTextureDesc normalDesc;
            {
                normalDesc.location = pNormal->getLocation();
                normalDesc.sampler = sampler;
                normalDesc.texture = textureNormal;
            }

            RHIUniformTextureDesc diffuseDesc;
            {
                diffuseDesc.location = pDiffuse->getLocation();
                diffuseDesc.sampler = sampler;
                diffuseDesc.texture = textureDiffuse;
            }

            auto uniformSet = device.createUniformSet({normalDesc,diffuseDesc}, {blockDesc});

            // Create draw list (list of draw commands) and fill it for LATER execution
            auto drawList = device.createDrawList();
            auto writeDrawList = [&](){
                RHIWindowPassOptions windowPass;
                {
                    windowPass.clearColor = Color4f{ 0.0f, 0.0f, 0.0f, 0.0f };
                    windowPass.viewport = Region2i{ 0, 0, window->getSize() };
                    windowPass.clearMask = { EClearOption::Color, EClearOption::Depth };
                }

                drawList->begin();
                drawList->bindWindow(window, windowPass);
                drawList->bindPipeline(pipelineState);
                drawList->bindUniformSet(uniformSet);
                drawList->bindArrayObject(array);
                drawList->drawIndexed(mesh->getIndexType(), mesh->getIndicesCount());
                drawList->end();
            };

            while (!window->shouldClose()) {
                writeDrawList();

                static float tX = 0;
                static float tY = 0;
                static float tZ = 0;
                static float dtX = 1.0f;
                static float dtY = 1.0f;
                static float dtZ = 1.0f;
                static float a = 0.0f;
                static float da = 1.0f;
                static float R = 50;

                auto t = Transformf().rotateY(a).translate({tX,tY,tZ});
                auto proj = Mat4x4f::perspective(Math::degToRad(90.f), window->getAspectRatio(), 0.01, 100.0f);
                auto view = Mat4x4f::lookAt({0,0,0}, {0,0,1}, {0,1,0});
                auto model = t.toTransformMat();
                auto norm = t.toNormalMat();
                auto lightPos = Vec3f(0,R * Math::sin(a), R * Math::cos(a));

                auto tproj = proj.transpose();
                auto tview = view.transpose();
                auto tmodel = model.transpose();
                auto tnorm = norm.transpose();

                a += da * engine.getFrameTime();

                uniformBuffer->update(pProj->getSize(), pProj->getOffset(), &tproj);
                uniformBuffer->update(pView->getSize(), pView->getOffset(), &tview);
                uniformBuffer->update(pModel->getSize(), pModel->getOffset(), &tmodel);
                uniformBuffer->update(pNorm->getSize(), pNorm->getOffset(), &tnorm);
                uniformBuffer->update(pLightPos->getSize(), pLightPos->getOffset(), &lightPos);

                main.execSingleIteration();

                device.beginRenderFrame();
                device.submitDrawList(drawList);
                device.endRenderFrame();

                if (input.isKeyPressed(EKeyboardKey::Right))
                    tX -= dtX;
                if (input.isKeyPressed(EKeyboardKey::Left))
                    tX += dtX;
                if (input.isKeyPressed(EKeyboardKey::Up))
                    tZ += dtX;
                if (input.isKeyPressed(EKeyboardKey::Down))
                    tZ -= dtX;

            }
        }
        main.finalize();
    };
}