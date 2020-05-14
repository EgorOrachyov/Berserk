/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <GLExtensions.h>
#include <Typedefs.h>
#include <glad/glad.h>
#include <String/TStringUtility.h>

namespace Berserk {

    GLExtensions::GLExtensions() {
        int32 extensionsCount;
        glGetIntegerv(GL_NUM_EXTENSIONS, &extensionsCount);

        for (int32 i = 0; i < extensionsCount; i++) {
            const GLubyte* ext = glGetStringi(GL_EXTENSIONS, i);

            if (CStringUtility::compare("GL_ARB_get_program_binary", (const char*) ext) == 0) {
                extGetProgramBinary = true;
            }
        }
    }

    GLExtensions& GLExtensions::getSingleton() {
        return *gExtensions;
    }

    GLExtensions* GLExtensions::gExtensions;

}