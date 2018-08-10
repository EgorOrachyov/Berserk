//
// Created by Egor Orachyov on 04.07.2018.
//

#ifndef BERSERK_GLRENDERSYSTEM_H
#define BERSERK_GLRENDERSYSTEM_H

#include "Pipeline/GLRenderPipeline.h"
#include "Render/RenderSystem.h"
#include "Render/GLScreenPlane.h"
#include "Render/GLRenderNode.h"

#include "Managers/GLSamplerManager.h"
#include "Managers/GLTextureManager.h"
#include "Managers/GLMaterialManager.h"
#include "Managers/GLRenderMeshManager.h"

namespace Berserk
{

    class GLRenderSystem : public RenderSystem
    {
    public:

        GLRenderSystem();
        virtual ~GLRenderSystem();

        void init(const ConfigTable& table) override;
        void destroy() override;
        void validate() override;

        void preMainLoop() override;
        void preUpdate() override;
        void postUpdate() override;
        void postMainLoop() override;

        void renderPass1(RenderManager *manager) override;
        void renderPass2(RenderManager *manager) override;

        const CString& getName() const override;
        const CString& getRenderName() const override;
        const CString& getShadingLanguageName() const override;

        void setClearColor(const Vector4f& color) override;
        const Vector4f& getClearColor() override;

        UINT32 getWindowWidth() const override;
        UINT32 getWindowHeight() const override;
        void   getWindowSize(UINT32 &width, UINT32 &height) const override;

        UINT32 getPixelWindowWidth() const override;
        UINT32 getPixelWindowHeight() const override;
        void   getPixelWindowSize(UINT32& width, UINT32& height) const override;

        UINT32 getWindowPosX() const override;
        UINT32 getWindowPosY() const override;
        void   getWindowPos(UINT32& posX, UINT32& posY) const override;

        void registerRenderCamera(Camera* camera) override;
        void registerLightSource(AmbientLight* light) override;

        void registerLightSource(SpotLight* light) override;
        void registerLightSource(PointLight* light) override;
        void registerLightSource(DirectionalLight* light) override;

        void unregisterLightSource(SpotLight *light) override;
        void unregisterLightSource(PointLight *light) override;
        void unregisterLightSource(DirectionalLight *light) override;

        Camera* getRenderCamera() override;
        AmbientLight* getAmbientLightSource() override;

        LinkedList<SpotLight*>& getSpotLightSources() override;
        LinkedList<PointLight*>& getPointLightSources() override;
        LinkedList<DirectionalLight*>& getDirectionalLightSources() override;

        GPUBuffer *createGPUBuffer(const CStaticString &name) override;

        RenderNode* createRenderNode() override;
        void deleteRenderNode(RenderNode* node) override;

        GLSamplerManager &getSamplerManagerRef();
        GLSamplerManager *getSamplerManagerPtr();

        TextureManager &getTextureManagerRef() override;
        TextureManager *getTextureManagerPtr() override;

        MaterialManager &getMaterialManagerRef() override;
        MaterialManager *getMaterialManagerPtr() override;

        RenderMeshManager &getRenderMeshManagerRef() override;
        RenderMeshManager *getRenderMeshManagerPtr() override;

    private:

        void printContextInfo() const;
        void getContextInfo();

        friend class GLToneMap;
        friend class GLScreenRender;
        friend class GLFragmentLightning;

        GLScreenPlane* getScreenPlane();
        GLFrameBufferObject* getStageInBuffer();
        GLFrameBufferObject* getStageOutBuffer();

    private:

        INT32 mWindowWidth;
        INT32 mWindowHeight;
        INT32 mPixelWindowWidth;
        INT32 mPixelWindowHeight;
        INT32 mWindowPosX;
        INT32 mWindowPosY;

        Vector4f mClearColor;
        FLOAT32  mExposure;
        FLOAT32  mGammaCorrection;

        Camera* mRenderCamera;
        AmbientLight* mAmbientLight;
        LinkedList<SpotLight*> mSpotLightSources;
        LinkedList<PointLight*> mPointLightSources;
        LinkedList<DirectionalLight*> mDirectionalLightSources;

        GLSamplerManager mSamplerManager;
        GLTextureManager mTextureManager;
        GLMaterialManager mMaterialManager;
        GLRenderMeshManager mRenderMeshManager;

        GLScreenPlane mScreenPlane;
        GLFrameBufferObject* mStageIn;
        GLFrameBufferObject* mStageOut;
        GLFrameBufferObject mRGB32FBuffer1;
        GLFrameBufferObject mRGB32FBuffer2;

        LinkedList<GLRenderNode> mRenderNodeList;

        GLFWwindow* mWindowHandle;

        GLRenderPipeline* mPreProcess;
        GLRenderPipeline* mMainProcess;
        GLRenderPipeline* mPostProcess;

        CString mName;
        CString mRenderName;
        CString mShadingLanguage;
    };

} // namespace Berserk

#endif //BERSERK_GLRENDERSYSTEM_H