//
// Created by Egor Orachyov on 04.07.2018.
//

#include "Render/GLRenderSystem.h"
#include "Buffers/GLGPUBuffer.h"
#include "Logging/LogMessages.h"
#include "Memory/MemoryAllocators.h"

#include "Pipeline/GLFragmentLightning.h"

namespace Berserk
{

    GLRenderSystem::GLRenderSystem()
    {
        mRenderCamera = nullptr;
        mAmbientLight = nullptr;
        mPreProcess = nullptr;
        mMainProcess = nullptr;
        mPostProcess = nullptr;
    }

    GLRenderSystem::~GLRenderSystem()
    {
        PUSH("Delete GL Render System %p\n", this);

        destroy();
    }

    void GLRenderSystem::init(const ConfigTable& table)
    {
        PUSH("Init GL Render System %p\n", this);

        if (!glfwInit())
        {
            ERROR("Cannot initialize GLFW library");
            return;
        }

        if (table.getUInt32("MultiSampleAntiAliasing"))
            glfwWindowHint(GLFW_SAMPLES, table.getUInt32("MultiSampleAntiAliasingDesiredValue"));
        else
            glfwWindowHint(GLFW_SAMPLES, 0);

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

        mWindowHandle = glfwCreateWindow(table.getUInt32("WindowWidth"),
                                         table.getUInt32("WindowHeight"),
                                         table.getChar("ApplicationName"),
                                         nullptr, nullptr);

        if (mWindowHandle == nullptr)
        {
            ERROR("Cannot create glfw window");
            return;
        }

        glfwMakeContextCurrent(mWindowHandle);
        glfwGetWindowPos(mWindowHandle, &mWindowPosX, &mWindowPosY);
        glfwGetWindowSize(mWindowHandle, &mWindowWidth, &mWindowHeight);
        glfwGetFramebufferSize(mWindowHandle, &mPixelWindowWidth, &mPixelWindowHeight);

        if (glewInit() != GLEW_OK)
        {
            ERROR("Cannot initialize GLEW library");
            return;
        }

        ////////////////////////////////////
        /// !!!WARNING!!! DEBUG TESTING  ///
        ////////////////////////////////////

        mPreProcess = nullptr;
        mMainProcess = new GLFragmentLightning(); mMainProcess->init();
        mPostProcess = nullptr;

        getContextInfo();
    }

    void GLRenderSystem::destroy()
    {
        if (mPreProcess)
        {
            mPreProcess->destroy();
            SAFE_DELETE(mPreProcess);
        }
        if (mMainProcess)
        {
            mMainProcess->destroy();
            SAFE_DELETE(mMainProcess);
        }
        if (mPostProcess)
        {
            mPostProcess->destroy();
            SAFE_DELETE(mPostProcess);
        }
        if (mWindowHandle)
        {
            glfwDestroyWindow(mWindowHandle);
            mWindowHandle = nullptr;
        }

        glfwTerminate();
    }

    void GLRenderSystem::validate()
    {
        PUSH("GLRenderSystem: validation");

        if (mRenderCamera == nullptr)
        { PUSH("Render Camera is not attached"); }
        else
        { PUSH("Attached render camera with name %s", mRenderCamera->getName().getChars()); }
    }

    void GLRenderSystem::preMainLoop()
    {
        PUSH("GLRenderSystem: pre-main loop stage");
    }

    void GLRenderSystem::preUpdate()
    {
        glfwGetWindowPos(mWindowHandle, &mWindowPosX, &mWindowPosY);
        glfwGetWindowSize(mWindowHandle, &mWindowWidth, &mWindowHeight);
        glfwGetFramebufferSize(mWindowHandle, &mPixelWindowWidth, &mPixelWindowHeight);
    }

    void GLRenderSystem::postUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(mWindowHandle);
    }

    void GLRenderSystem::postMainLoop()
    {
        PUSH("GLRenderSystem: post-main loop stage");
    }

    void GLRenderSystem::renderPass1(RenderManager *manager)
    {
        GLFrameBufferObject *fbo = nullptr;

        if (mPreProcess)
        {
            fbo = mPreProcess->process(manager, nullptr);
        }

        if (mMainProcess)
        {
            fbo = mMainProcess->process(manager, fbo);
        }

        if (mPostProcess)
        {
            fbo = mPostProcess->process(manager, fbo);
        }
    }

    void GLRenderSystem::renderPass2(RenderManager *manager)
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

    void GLRenderSystem::setClearColor(const Vector4f &color)
    {
        mClearColor = color;
    }

    UINT32 GLRenderSystem::getWindowWidth() const
    {
        return (UINT32)mWindowWidth;
    }

    UINT32 GLRenderSystem::getWindowHeight() const
    {
        return (UINT32)mWindowHeight;
    }

    void GLRenderSystem::getWindowSize(UINT32 &width, UINT32 &height) const
    {
        width = (UINT32)mWindowWidth;
        height = (UINT32)mWindowHeight;
    }

    UINT32 GLRenderSystem::getPixelWindowWidth() const
    {
        return (UINT32)mPixelWindowWidth;
    }

    UINT32 GLRenderSystem::getPixelWindowHeight() const
    {
        return (UINT32)mPixelWindowHeight;
    }

    void GLRenderSystem::getPixelWindowSize(UINT32& width, UINT32& height) const
    {
        width = (UINT32)mPixelWindowWidth;
        height = (UINT32)mPixelWindowHeight;
    }

    UINT32 GLRenderSystem::getWindowPosX() const
    {
        return (UINT32)mWindowPosX;
    }

    UINT32 GLRenderSystem::getWindowPosY() const
    {
        return (UINT32)mWindowPosY;
    }

    void GLRenderSystem::getWindowPos(UINT32 &posX, UINT32 &posY) const
    {
        posX = (UINT32)mWindowPosX;
        posY = (UINT32)mWindowPosY;
    }

    void GLRenderSystem::registerRenderCamera(Camera* camera)
    {
        ASSERT(camera, "GLRenderSystem: Attempt to pass nullptr render camera");
        mRenderCamera = camera;
        mRenderCamera->setViewport(0, 0, (UINT32)mPixelWindowWidth, (UINT32)mPixelWindowHeight);
    }

    void GLRenderSystem::registerLightSource(SpotLight* light)
    {
        ASSERT(light, "GLRenderSystem: Attempt to pass nullptr spot light source");
        mSpotLightSources.add(light);
    }

    void GLRenderSystem::registerLightSource(PointLight* light)
    {
        ASSERT(light, "GLRenderSystem: Attempt to pass nullptr point light source");
        mPointLightSources.add(light);
    }

    void GLRenderSystem::registerLightSource(AmbientLight* light)
    {
        ASSERT(light, "GLRenderSystem: Attempt to pass nullptr ambient light source");
        mAmbientLight = light;
    }

    void GLRenderSystem::registerLightSource(DirectionalLight* light)
    {
        ASSERT(light, "GLRenderSystem: Attempt to pass nullptr directional light source");
        mDirectionalLightSources.add(light);
    }

    void GLRenderSystem::deleteLightSource(SpotLight* light)
    {
        ASSERT(light, "GLRenderSystem: Attempt to pass nullptr spot light source");
        for(UINT32 i = 0; i < mSpotLightSources.getSize(); i++)
        {
            if (mSpotLightSources.get(i) == light)
            {
                mSpotLightSources.removeIgnoreOrder(i);
                return;
            }
        }
    }

    void GLRenderSystem::deleteLightSource(PointLight* light)
    {
        ASSERT(light, "GLRenderSystem: Attempt to pass nullptr point light source");
        for(UINT32 i = 0; i < mPointLightSources.getSize(); i++)
        {
            if (mPointLightSources.get(i) == light)
            {
                mPointLightSources.removeIgnoreOrder(i);
                return;
            }
        }
    }

    void GLRenderSystem::deleteLightSource(DirectionalLight* light)
    {
        ASSERT(light, "GLRenderSystem: Attempt to pass nullptr directional light source");
        for(UINT32 i = 0; i < mDirectionalLightSources.getSize(); i++)
        {
            if (mDirectionalLightSources.get(i) == light)
            {
                mDirectionalLightSources.removeIgnoreOrder(i);
                return;
            }
        }
    }

    Camera* GLRenderSystem::getRenderCamera()
    {
        return mRenderCamera;
    }

    AmbientLight* GLRenderSystem::getAmbientLightSource()
    {
        return mAmbientLight;
    }

    ArrayList<SpotLight*>& GLRenderSystem::getSpotLightSources()
    {
        return mSpotLightSources;
    }

    ArrayList<PointLight*>& GLRenderSystem::getPointLightSources()
    {
        return mPointLightSources;
    }

    ArrayList<DirectionalLight*>& GLRenderSystem::getDirectionalLightSources()
    {
        return mDirectionalLightSources;
    }

    GPUBuffer* GLRenderSystem::createGPUBuffer(const CStaticString &name)
    {
        return new GLGPUBuffer();
    }

    GLSamplerManager& GLRenderSystem::getSamplerManagerRef()
    {
        return mSamplerManager;
    }

    GLSamplerManager* GLRenderSystem::getSamplerManagerPtr()
    {
        return &mSamplerManager;
    }

    TextureManager& GLRenderSystem::getTextureManagerRef()
    {
        return mTextureManager;
    }

    TextureManager* GLRenderSystem::getTextureManagerPtr()
    {
        return &mTextureManager;
    }

    MaterialManager& GLRenderSystem::getMaterialManagerRef()
    {
        return mMaterialManager;
    }

    MaterialManager* GLRenderSystem::getMaterialManagerPtr()
    {
        return &mMaterialManager;
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