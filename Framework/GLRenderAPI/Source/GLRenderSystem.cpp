//
// Created by Egor Orachyov on 04.07.2018.
//

#include "Render/GLRenderSystem.h"
#include "Buffers/GLGPUBuffer.h"
#include "Logging/LogMessages.h"
#include "Memory/MemoryAllocators.h"

#include "Pipeline/GLPhongModel.h"
#include "Pipeline/GLToneMap.h"
#include "Pipeline/GLGaussianBloom.h"
#include "Pipeline/GLScreenRender.h"

namespace Berserk
{

    GLRenderSystem::GLRenderSystem()
    {
        mRenderCamera = nullptr;
        mAmbientLight = Vector3f(0);
    }

    GLRenderSystem::~GLRenderSystem()
    {
        destroy();
    }

    void GLRenderSystem::init(const ConfigTable& table)
    {
        if (!glfwInit())
        {
            ERROR("Cannot initialize GLFW library");
            return;
        }

        if (table.getUInt32("MultiSampleAntiAliasing"))
        { glfwWindowHint(GLFW_SAMPLES, table.getUInt32("MultiSampleAntiAliasingDesiredValue")); }
        else
        { glfwWindowHint(GLFW_SAMPLES, 0); }

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

        if (table.getUInt32("WindowMaximize"))
        { glfwMaximizeWindow(mWindowHandle); }

        glfwMakeContextCurrent(mWindowHandle);
        glfwGetWindowPos(mWindowHandle, &mWindowPosX, &mWindowPosY);
        glfwGetWindowSize(mWindowHandle, &mWindowWidth, &mWindowHeight);
        glfwGetFramebufferSize(mWindowHandle, &mPixelWindowWidth, &mPixelWindowHeight);

        mOldPixelWindowWidth = mPixelWindowWidth;
        mOldPixelWindowHeight = mPixelWindowHeight;

        if (glewInit() != GLEW_OK)
        {
            ERROR("Cannot initialize GLEW library");
            return;
        }

        mStageIn = nullptr;
        mStageOut = nullptr;

        mRenderCamera = nullptr;
        mAmbientLight = Vector3f(0);
        mExposure = table.getFloat32("Exposure");
        mLuminanceThresh = table.getFloat32("LuminanceThresh");
        mGammaCorrection = (FLOAT32)1 / table.getFloat32("GammaCorrection");

        mWasReSized = false;

        mSpotLightSources.init(LightInfo::LI_MAX_SPOT_LIGHTS);
        mPointLightSources.init(LightInfo::LI_MAX_POINT_LIGHTS);
        mDirectionalLightSources.init(LightInfo::LI_MAX_DIRECTIONAL_LIGHTS);
        mRenderNodeSources.init();

        mSpotLightSources.lock();
        mPointLightSources.lock();
        mDirectionalLightSources.lock();

        mRenderNodeList.init();

        mScreenPlane.init();

        mPhongModelStage = new GLPhongModel();
        mPhongModelStage->init();

        mToneMapStage = new GLToneMap();
        mToneMapStage->init();

        mGaussianBloomStage = new GLGaussianBloom();
        mGaussianBloomStage->init();

        mScreenRenderStage = new GLScreenRender();
        mScreenRenderStage->init();

        mRGB32FBuffer1.init((UINT32)mPixelWindowWidth, (UINT32)mPixelWindowHeight);
        mRGB32FBuffer1.addTexture(GLInternalTextureFormat::GLTF_RGB32F, GLWrapping::GLW_CLAMP_TO_EDGE, GLFiltering::GLF_NEAREST, 0, 0);
        mRGB32FBuffer1.addDepthBuffer();
        mRGB32FBuffer1.setShaderAttachments();

        mRGB32FBuffer2.init((UINT32)mPixelWindowWidth, (UINT32)mPixelWindowHeight);
        mRGB32FBuffer2.addTexture(GLInternalTextureFormat::GLTF_RGB32F, GLWrapping::GLW_CLAMP_TO_EDGE, GLFiltering::GLF_NEAREST, 0, 0);
        mRGB32FBuffer2.addDepthBuffer();
        mRGB32FBuffer2.setShaderAttachments();

        getContextInfo();
    }

    void GLRenderSystem::destroy()
    {
        if (mWindowHandle)
        {
            glfwDestroyWindow(mWindowHandle);
            mWindowHandle = nullptr;
        }
        if (mPhongModelStage)
        {
            mPhongModelStage->destroy();
            SAFE_DELETE(mPhongModelStage);
        }
        if (mToneMapStage)
        {
            mToneMapStage->destroy();
            SAFE_DELETE(mToneMapStage);
        }
        if (mGaussianBloomStage)
        {
            mGaussianBloomStage->destroy();
            SAFE_DELETE(mGaussianBloomStage);
        }
        if (mScreenRenderStage)
        {
            mScreenRenderStage->destroy();
            SAFE_DELETE(mScreenRenderStage);
        }

        mRGB32FBuffer1.destroy();
        mRGB32FBuffer2.destroy();

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

        /////////////////
        ///   DEBUG   ///
        /////////////////

        static auto current = glfwGetTime();
        static auto elapsed = glfwGetTime();

        auto tmp = glfwGetTime();
        elapsed = tmp - current;
        current = tmp;

        printf("FPS %2.1lf\n",1 / elapsed);
    }

    void GLRenderSystem::postUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(mWindowHandle);

        if (mOldPixelWindowWidth != mPixelWindowWidth || mOldPixelWindowHeight != mOldPixelWindowHeight)
        {
            PUSH("Was re-sized");

            mRGB32FBuffer1.destroy();
            mRGB32FBuffer1.init((UINT32)mPixelWindowWidth, (UINT32)mPixelWindowHeight);
            mRGB32FBuffer1.addTexture(GLInternalTextureFormat::GLTF_RGB32F, GLWrapping::GLW_CLAMP_TO_EDGE, GLFiltering::GLF_NEAREST, 0, 0);
            mRGB32FBuffer1.addDepthBuffer();
            mRGB32FBuffer1.setShaderAttachments();

            mRGB32FBuffer2.destroy();
            mRGB32FBuffer2.init((UINT32)mPixelWindowWidth, (UINT32)mPixelWindowHeight);
            mRGB32FBuffer2.addTexture(GLInternalTextureFormat::GLTF_RGB32F, GLWrapping::GLW_CLAMP_TO_EDGE, GLFiltering::GLF_NEAREST, 0, 0);
            mRGB32FBuffer2.addDepthBuffer();
            mRGB32FBuffer2.setShaderAttachments();

            mOldPixelWindowWidth = mPixelWindowWidth;
            mOldPixelWindowHeight = mPixelWindowHeight;

            mWasReSized = true;
        }
        else
        {
            mWasReSized = false;
        }
    }

    void GLRenderSystem::postMainLoop()
    {
        PUSH("GLRenderSystem: post-main loop stage");
    }

    void GLRenderSystem::renderPass1()
    {
        GLFrameBufferObject* tmp;

        mStageIn = &mRGB32FBuffer2;
        mStageOut = &mRGB32FBuffer1;
        mPhongModelStage->execute();

        tmp = mStageIn;
        mStageIn = mStageOut;
        mStageOut = tmp;
        mGaussianBloomStage->execute();

        tmp = mStageIn;
        mStageIn = mStageOut;
        mStageOut = tmp;
        mToneMapStage->execute();

        tmp = mStageIn;
        mStageIn = mStageOut;
        mStageOut = tmp;
        mScreenRenderStage->execute();

        mSpotLightSources.clean();
        mPointLightSources.clean();
        mDirectionalLightSources.clean();
        mRenderNodeSources.clean();
    }

    void GLRenderSystem::renderPass2()
    {
        mSpotLightSources.clean();
        mPointLightSources.clean();
        mDirectionalLightSources.clean();
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

    void GLRenderSystem::setRenderCamera(Camera *camera)
    {
        ASSERT(camera, "GLRenderSystem: Attempt to pass nullptr render camera");
        mRenderCamera = camera;
        mRenderCamera->setViewport(0, 0, (UINT32)mPixelWindowWidth, (UINT32)mPixelWindowHeight);
    }

    void GLRenderSystem::setAmbientLight(const Vector3f& light)
    {
        mAmbientLight = light;
    }

    void GLRenderSystem::setClearColor(const Vector4f &color)
    {
        mClearColor = color;
    }

    Camera* GLRenderSystem::getRenderCamera()
    {
        return mRenderCamera;
    }

    const Vector3f& GLRenderSystem::getAmbientLightSource()
    {
        return mAmbientLight;
    }

    const Vector4f& GLRenderSystem::getClearColor()
    {
        return mClearColor;
    }

    void GLRenderSystem::setExposure(FLOAT32 exposure)
    {
        mExposure = exposure;
    }

    void GLRenderSystem::setLuminanceThresh(FLOAT32 luminance)
    {
        mLuminanceThresh = luminance;
    }

    void GLRenderSystem::setGammaCorrection(FLOAT32 gamma)
    {
        mGammaCorrection = (FLOAT32)1.0 / gamma;
    }

    FLOAT32 GLRenderSystem::getExposure()
    {
        return mExposure;
    }

    FLOAT32 GLRenderSystem::getLuminanceThresh()
    {
        return mLuminanceThresh;
    }

    FLOAT32 GLRenderSystem::getGammaCorrection()
    {
        return (FLOAT32)1.0 / mGammaCorrection;
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

    UINT32 GLRenderSystem::getOldPixelWindowWidth() const
    {
        return (UINT32)mOldPixelWindowWidth;
    }

    UINT32 GLRenderSystem::getOldPixelWindowHeight() const
    {
        return (UINT32)mOldPixelWindowHeight;
    }

    void GLRenderSystem::getOldPixelWindowSize(UINT32& width, UINT32& height) const
    {
        width = (UINT32)mOldPixelWindowWidth;
        height = (UINT32)mOldPixelWindowHeight;
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

    bool GLRenderSystem::wasResized()
    {
        return mWasReSized;
    }

    void GLRenderSystem::queueLightSource(SpotLight* light)
    {
        ASSERT(light, "GLRenderSystem: An attempt to pass NULL spot light");
        mSpotLightSources.add(light);
    }

    void GLRenderSystem::queueLightSource(PointLight* light)
    {
        ASSERT(light, "GLRenderSystem: An attempt to pass NULL point light");
        mPointLightSources.add(light);
    }

    void GLRenderSystem::queueLightSource(DirectionalLight* light)
    {
        ASSERT(light, "GLRenderSystem: An attempt to pass NULL directional light");
        mDirectionalLightSources.add(light);
    }

    void GLRenderSystem::queueRenderNode(RenderNode *node)
    {
        ASSERT(node, "GLRenderSystem: An attempt to pass NULL render node");
        mRenderNodeSources.add(node);
    }

    List<SpotLight*>& GLRenderSystem::getSpotLightSources()
    {
        return mSpotLightSources;
    }

    List<PointLight*>& GLRenderSystem::getPointLightSources()
    {
        return mPointLightSources;
    }

    List<DirectionalLight*>& GLRenderSystem::getDirectionalLightSources()
    {
        return mDirectionalLightSources;
    }

    List<RenderNode *> & GLRenderSystem::getRenderNodeSources()
    {
        return mRenderNodeSources;
    }

    RenderNode* GLRenderSystem::createRenderNode()
    {
        GLRenderNode node;
        mRenderNodeList.add(node);
        return &mRenderNodeList.getLast();
    }

    void GLRenderSystem::deleteRenderNode(RenderNode* node)
    {
        mRenderNodeList.iterate(true);
        while (mRenderNodeList.iterate())
        {
            if (mRenderNodeList.getCurrent() == *dynamic_cast<GLRenderNode*>(node))
            {
                PUSH("Delete Render Node: %p", node);
                node->destroy();
                mRenderNodeList.remove(*dynamic_cast<GLRenderNode*>(node));
                return;
            }
        }
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

    RenderMeshManager& GLRenderSystem::getRenderMeshManagerRef()
    {
        return mRenderMeshManager;
    }

    RenderMeshManager* GLRenderSystem::getRenderMeshManagerPtr()
    {
        return &mRenderMeshManager;
    }

    GLScreenPlane* GLRenderSystem::getScreenPlane()
    {
        return &mScreenPlane;
    }

    GLFrameBufferObject* GLRenderSystem::getStageInBuffer()
    {
        return mStageIn;
    }

    GLFrameBufferObject* GLRenderSystem::getStageIn2Buffer()
    {
        return mStageIn2;
    }

    GLFrameBufferObject* GLRenderSystem::getStageOutBuffer()
    {
        return mStageOut;
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