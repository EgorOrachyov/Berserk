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
        auto printStat = [](ShaderFile& shaderFile){
            if (shaderFile.isFileParsed()) {
                printf("Info about %s:\n", shaderFile.getShaderName().data());
                printf("Version: %s\n", shaderFile.getStringVersion().data());
                printf("Author: %s\n", shaderFile.getAuthorName().data());
                printf("Description: %s\n", shaderFile.getShaderDescription().data());
                printf("Type: %s\n", ShaderFile::getShaderFileTypeStringFromEnum(shaderFile.getShaderType()));

                static CString OpenGL = "OpenGL";

                if (shaderFile.supportsDevice(OpenGL)) {
                    printf("Supports OpenGL\n");

                    auto types = shaderFile.getShaderTypesForDevice(OpenGL);
                    auto names = shaderFile.getShaderNamesForDevice(OpenGL);

                    for (uint32 i = 0; i < types.size(); i++) {
                        printf("Shader %s [type: %s]\n", names[i].data(), RHIDefinitionsUtil::getShaderTypeStringFromEnum(types[i]));
                    }
                }
            }
        };

        ShaderFile file1("/Shaders/TestShader.json", EPathType::Engine);
        printStat(file1);

        ShaderFile file2("/Shaders/TestDependency.json", EPathType::Engine);
        printStat(file2);
    };
}