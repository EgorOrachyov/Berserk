//
// Created by Egor Orachyov on 04.07.2018.
//

#include <Strings/WString.h>
#include "System/GLRenderSystem.h"
#include "Logging/LogMessages.h"
#include "Memory/MemoryAllocators.h"

namespace Berserk
{

    GLRenderSystem::GLRenderSystem()
    {
        mPipeline = NULL;
    }

    GLRenderSystem::~GLRenderSystem()
    {
        PUSH("Delete GL Render System %p\n", this);

        destroy();
    }

    void GLRenderSystem::init(const ConfigTable& table)
    {
        PUSH("Init GL Render System %p\n", this);

        /// Init glfw as window system and opengl context handle

        if (!glfwInit())
        {
            ERROR("Cannot initialize GLFW library");
            return;
        }

        /// Use MSAA algorithm and define num of buffers

        if (table.getUInt32("MultiSampleAntiAliasing"))
        {
            glfwWindowHint(GLFW_SAMPLES, table.getUInt32("MultiSampleAntiAliasingDesiredValue"));
        }
        else
        {
            glfwWindowHint(GLFW_SAMPLES, 0);
        }

        /// Hints to get last context version for target platform

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

        /// Create window (handles viewport and events)

        mWindow.create(table.getUInt32("WindowWidth"), table.getUInt32("WindowHeight"), table.getChar("ApplicationName"));
        mWindow.makeCurrent();

        /// Init glew as opengl initializer

        if (glewInit() != GLEW_OK)
        {
            ERROR("Cannot initialize GLEW library");
            return;
        }

        /// Init pipeline

        // todo

        /// Setup internal values

        mName = CString("Open GL Rendering System");
        mRenderName = CString("Open GL Graphic driver 4.1");
        mShadingLanguage = CString("GLSL 4.1");
    }

    void GLRenderSystem::destroy()
    {
        if (mPipeline)
        {
            mPipeline->destroy();
            SAFE_DELETE(mPipeline);
        }

        mWindow.destroy();

        glfwTerminate();
    }

    void GLRenderSystem::validate()
    {

    }

    void GLRenderSystem::preMainLoop()
    {

    }

    void GLRenderSystem::preUpdate()
    {

    }

    void GLRenderSystem::postUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(mWindow.getHandle());
    }

    void GLRenderSystem::postMainLoop()
    {

    }

    const CString& GLRenderSystem::getName() const
    {
        return mName;
    }

    const CString& GLRenderSystem::getRenderName() const
    {
        return mRenderName;
    }

    const CString& GLRenderSystem::getShadingLanguageName() const
    {
        return mShadingLanguage;
    }

    void GLRenderSystem::printContextInfo() const
    {
        // debug only feature

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