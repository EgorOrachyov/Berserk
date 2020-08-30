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
#include <Resources/Mesh.h>
#include <Resources/MeshImportOptions.h>
#include <ShaderManager.h>
#include <Material.h>
#include <MaterialBuilder.h>
#include <UniformBlockLayout.h>
#include <UniformBlockLayoutBuilder.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestRenderMaterial)
{
    BERSERK_TEST_COND(RenderMaterial, true)
    {
        Main main;
        main.initialize(0, nullptr);
        {
            // References to common engine singletons
            auto& system = System::getSingleton();
            auto& engine = Engine::getSingleton();
            auto& input = Input::getSingleton();
            auto& device = RHIDevice::getSingleton();
            auto& shaderManager = Render::ShaderManager::getSingleton();
            auto& windowManager = WindowManager::getSingleton();
            auto& importers = ResourceImporters::getSingleton();

            // Default window, created on engine start-up
            auto window = windowManager.find("MAIN_WINDOW");

            const char vs[] =
                    "#version 410 core\n"
                    "layout (location = 0) in vec3 p;"
                    ""
                    "layout (std140) uniform Tr {"
                    "  vec2  f2;"
                    "  vec3  f3;"
                    "  float f1;"
                    "  vec4  f4;"
                    "  mat3  m3;"
                    "  mat4  m4;"
                    "  mat2  m2;"
                    "  float arf1[2];"
                    "  vec2  arf2[2];"
                    "  vec3  arf3[2];"
                    "  vec4  arf4[2];"
                    "  mat2  arm2[2];"
                    "  mat3  arm3[2];"
                    "  mat4  arm4[2];"
                    "};"
                    "void main() {"
                    " gl_Position = vec4(p, 1.0f);"
                    "}";

            const char fs[] =
                    "#version 410 core\n"
                    "layout (location = 0) out vec4 c;"
                    "void main() {"
                    " c = vec4(1.0f);"
                    "}";

            RHIShaderDesc shaderDesc;
            {
                shaderDesc.resize(2);
                shaderDesc[0].type = EShaderType::Vertex;
                shaderDesc[0].code.add((uint8*) vs, CStringUtility::length(vs));
                shaderDesc[1].type = EShaderType::Fragment;
                shaderDesc[1].code.add((uint8*) fs, CStringUtility::length(fs));
            }

            auto shader = device.createShader(EShaderLanguage::GLSL, shaderDesc);
            auto shaderMeta = device.createShaderMeta(shader);

            shaderMeta->showDebugInfo();

            Render::UniformBlockLayout layout;
            Render::UniformBlockLayoutBuilder builder;

            builder
             .setName("Tr")
             .addMember("f2", EShaderData::Float2)
             .addMember("f3", EShaderData::Float3)
             .addMember("f1", EShaderData::Float1)
             .addMember("f4", EShaderData::Float4)
             .addMember("m3", EShaderData::Mat3)
             .addMember("m4", EShaderData::Mat4)
             .addMember("m2", EShaderData::Mat2)
             .addMemberArray("arf1", EShaderData::Float1, 2)
             .addMemberArray("arf2", EShaderData::Float2, 2)
             .addMemberArray("arf3", EShaderData::Float3, 2)
             .addMemberArray("arf4", EShaderData::Float4, 2)
             .addMemberArray("arm2", EShaderData::Mat2, 2)
             .addMemberArray("arm3", EShaderData::Mat3, 2)
             .addMemberArray("arm4", EShaderData::Mat4, 2)
             .buildMove(layout);

            layout.showDebugInfo();

            while (!window->shouldClose()) {
                main.execSingleIteration();
            }
        }
        main.finalize();
    };
}