//
// Created by Egor Orachyov on 04.07.2018.
//

#include "Context/GLContext.h"
#include "../../Core/Essential/Assert.h"
#include "../../Core/Logging/LogMessages.h"

#include "Essential/GLInclude.h"

namespace Berserk
{

    GLContext::GLContext()
    {
        mIsInitialized = false;
    }

    GLContext::~GLContext()
    {
        destroy();
    }

    void GLContext::initWindowContext()
    {
        if (mIsInitialized)
        {
            return;
        }

        int32 glfwResult = glfwInit();
        ASSERT(glfwResult, "Cannot initialize GLFW library");
        if (!glfwResult)
        {
            ERROR("Cannot initialize GLFW library");
            mIsInitialized = false;
            return;
        }

        #ifdef TARGET_PLATFORM_MACOS
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        #else
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #endif
    }

    void GLContext::initRenderingContext()
    {
        if (mIsInitialized)
        {
            return;
        }
        
        int32 glewResult = glewInit();
        ASSERT(glewResult == GLEW_OK, "Cannot initialize GLEW library");
        if (glewResult != GLEW_OK)
        {
            ERROR("Cannot initialize GLEW library");
            mIsInitialized = false;
            return;
        }
        
        mIsInitialized = true;
    }
    
    void GLContext::destroy()
    {
        if (mIsInitialized)
        {
            glfwTerminate();
            mIsInitialized = false;
        }
    }

    bool GLContext::isInitialized() const
    {
        return mIsInitialized;
    }

    void GLContext::printContextInfo() const
    {
        const GLubyte * renderer = glGetString(GL_RENDERER);
        const GLubyte * vendor = glGetString(GL_VENDOR);
        const GLubyte * version = glGetString(GL_VERSION);
        const GLubyte * glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

        GLint minor, major;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);

        OPEN_BLOCK("GL Context info");
        PUSH_BLOCK("GL Vendor            : %s", vendor);
        PUSH_BLOCK("GL Renderer          : %s", renderer);
        PUSH_BLOCK("GL Version (string)  : %s", version);
        PUSH_BLOCK("GL Version (integer) : %d.%d", major, minor);
        PUSH_BLOCK("GLSL Version         : %s", glslVersion);
        CLOSE_BLOCK();

        GLint numOfExtensions;
        glGetIntegerv(GL_NUM_EXTENSIONS, &numOfExtensions);

        OPEN_BLOCK("GL Extensions");
        for(int i = 0; i < numOfExtensions; i++)
        {
            PUSH_BLOCK("%s", glGetStringi(GL_EXTENSIONS, i));
        }
        CLOSE_BLOCK();
    }

} // namespace Berserk
