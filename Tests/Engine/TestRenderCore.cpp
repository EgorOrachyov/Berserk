/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>
#include <ShaderCore/ShaderFile.h>

using namespace Berserk;
using namespace Rendering;

BERSERK_TEST_SECTION(TestRenderCore)
{
    BERSERK_TEST_COND(ShaderFile, true)
    {
        ShaderFile shaderFile("/Engine/Shaders/TestShader.json");

        if (shaderFile.isFileParsed()) {
            printf("Info about %s:\n", shaderFile.getShaderName().data());
            printf("Version: %s\n", shaderFile.getStringVersion().data());
            printf("Author: %s\n", shaderFile.getAuthorName().data());
            printf("Description: %s\n", shaderFile.getShaderDescription().data());

            if (shaderFile.supportsDevice("OpenGL")) {
                printf("Supports OpenGL\n");
                for (auto& file: shaderFile.getShaderNamesForDevice("OpenGL"))
                    printf("Shader path: %s\n", file.data());
            }
        }
    };
}