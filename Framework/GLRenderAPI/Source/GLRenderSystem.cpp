//
// Created by Egor Orachyov on 04.07.2018.
//

#include "System/GLRenderSystem.h"
#include "Buffers/GLGPUBuffer.h"
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
        getContextInfo();
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

    void GLRenderSystem::beginRenderPass1()
    {

    }

    void GLRenderSystem::endRenderPass1()
    {

    }

    void GLRenderSystem::beginRenderPass2()
    {

    }

    void GLRenderSystem::endRenderPass2()
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

    void GLRenderSystem::setViewportBorders(UINT32 cinematic)
    {

    }

    void GLRenderSystem::setViewportBorders(UINT32 left, UINT32 right, UINT32 bottom, UINT32 top)
    {

    }

    UINT32 GLRenderSystem::getWindowWidth() const
    {

    }

    UINT32 GLRenderSystem::getWindowHeight() const
    {

    }

    GPUBuffer* GLRenderSystem::createGPUBuffer(const CStaticString &name)
    {
        return new GLGPUBuffer(name);
    }

    void GLRenderSystem::getContextInfo()
    {
        const GLubyte * renderer = glGetString(GL_RENDERER);
        const GLubyte * vendor = glGetString(GL_VENDOR);
        const GLubyte * version = glGetString(GL_VERSION);
        const GLubyte * glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

        GLint minor, major;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);

        CHAR buffer[BUFFER_SIZE_512] = {'\0'};

        sprintf(buffer, "%s %s", renderer, version);
        mName = CString((const CHAR*)buffer);

        sprintf(buffer, "%s", vendor);
        mRenderName = CString((const CHAR*)buffer);

        sprintf(buffer, "GLSL %s", glslVersion);
        mShadingLanguage = CString((const CHAR*)buffer);
    }

} // namespace Berserk