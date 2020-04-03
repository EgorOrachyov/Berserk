/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLERRORS_H
#define BERSERK_GLERRORS_H

#include <ErrorMacro.h>
#include <glad/glad.h>

namespace Berserk {

    class GLErrors {
    public:

        static void catchErrors() {
            GLenum error;
            while ((error = glGetError()) != GL_NO_ERROR) {
                BERSERK_ERROR("OpenGL RHI: %s", getErrorDescription(error));
            }
        }

        static const char* getErrorDescription(GLenum error) {
            switch (error) {
                case GL_INVALID_ENUM:
                    return "Invalid enum passed";
                case GL_INVALID_VALUE:
                    return "Invalid value passed";
                case GL_INVALID_OPERATION:
                    return "Set of state for a command is not legal for the parameters given to command";
                case GL_OUT_OF_MEMORY:
                    return "Out of memory";
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    return "Invalid framebuffer operation";
                default:
                    return "";
            }
        }

    };

#define BERSERK_CATCH_OPENGL_ERRORS() {                                                         \
        GLenum error;                                                                           \
        while ((error = glGetError()) != GL_NO_ERROR) {                                         \
            BERSERK_ERROR("OpenGL RHI: %s", GLErrors::getErrorDescription(error));              \
        }                                                                                       \
    }

}

#endif //BERSERK_GLERRORS_H
