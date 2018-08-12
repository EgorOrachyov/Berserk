//
// Created by Egor Orachyov on 04.07.2018.
//

#ifndef BERSERK_GLRENDERSYSTEM_H
#define BERSERK_GLRENDERSYSTEM_H

#include "Render/RenderSystem.h"
#include "Render/PipelineStage.h"
#include "Render/GLScreenPlane.h"
#include "Render/GLRenderNode.h"

#include "Buffers/GLFrameBufferObject.h"
#include "Buffers/GLDepthMap.h"

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

        void renderPass1() override;
        void renderPass2() override;

        const CString& getName() const override;
        const CString& getRenderName() const override;
        const CString& getShadingLanguageName() const override;

        void setRenderCamera(Camera *camera) override;
        void setAmbientLight(const Vector3f& light) override;
        void setClearColor(const Vector4f& color) override;

        Camera* getRenderCamera() override;
        const Vector3f& getAmbientLightSource() override;
        const Vector4f& getClearColor() override;

        void setExposure(FLOAT32 exposure) override;
        void setLuminanceThresh(FLOAT32 luminance) override;
        void setGammaCorrection(FLOAT32 gamma) override;

        FLOAT32 getExposure() override;
        FLOAT32 getLuminanceThresh() override;
        FLOAT32 getGammaCorrection() override;

        UINT32 getWindowWidth() const override;
        UINT32 getWindowHeight() const override;
        void   getWindowSize(UINT32 &width, UINT32 &height) const override;

        UINT32 getPixelWindowWidth() const override;
        UINT32 getPixelWindowHeight() const override;
        void   getPixelWindowSize(UINT32& width, UINT32& height) const override;

        UINT32 getOldPixelWindowWidth() const override;
        UINT32 getOldPixelWindowHeight() const override;
        void   getOldPixelWindowSize(UINT32& width, UINT32& height) const override;
        
        UINT32 getWindowPosX() const override;
        UINT32 getWindowPosY() const override;
        void   getWindowPos(UINT32& posX, UINT32& posY) const override;

        bool wasReSized() override;

        void queueShadowLightSource(SpotLight* light) override;
        void queueShadowLightSource(PointLight* light) override;
        void queueShadowLightSource(DirectionalLight* light) override;
        void queueLightSource(SpotLight* light) override;
        void queueLightSource(PointLight* light) override;
        void queueLightSource(DirectionalLight* light) override;
        void queueRenderNode(RenderNode* node) override;
        
        List<Light*>            &getDirectionalSources() override;
        List<Light*>            &getOmnidirectionalSources() override;
        List<SpotLight*>        &getSpotLightSources() override;
        List<PointLight*>       &getPointLightSources() override;
        List<DirectionalLight*> &getDirectionalLightSources() override;
        List<RenderNode*>       &getRenderNodeSources() override;

        DepthMap* getDepthMaps() override;
        CubeDepthMap* getCubeDepthMaps() override;

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

        GLScreenPlane* getScreenPlane();
        GLFrameBufferObject* getStageOutBuffer();
        GLFrameBufferObject* getStageInBuffer();
        GLFrameBufferObject* getStageIn2Buffer();

    private:

        void printContextInfo() const;
        void getContextInfo();


    private:

        INT32 mWindowWidth;
        INT32 mWindowHeight;
        INT32 mPixelWindowWidth;
        INT32 mPixelWindowHeight;
        INT32 mOldPixelWindowWidth;
        INT32 mOldPixelWindowHeight;
        INT32 mWindowPosX;
        INT32 mWindowPosY;

        INT8 mWasReSized : 1;

        Vector3f mAmbientLight;
        Vector4f mClearColor;
        FLOAT32  mLuminanceThresh;
        FLOAT32  mExposure;
        FLOAT32  mGammaCorrection;

        Camera*                 mRenderCamera;
        List<Light*>            mDirectionalSources;
        List<Light*>            mOmnidirectionalSources;
        List<SpotLight*>        mSpotLightSources;
        List<PointLight*>       mPointLightSources;
        List<DirectionalLight*> mDirectionalLightSources;
        List<RenderNode*>       mRenderNodeSources;

        GLDepthMap mDirectionalDepthMap[ShadowInfo::SI_MAX_DIRECTIONAL_SHADOWS];
        // todo: cube maps

        GLSamplerManager    mSamplerManager;
        GLTextureManager    mTextureManager;
        GLMaterialManager   mMaterialManager;
        GLRenderMeshManager mRenderMeshManager;

        GLScreenPlane           mScreenPlane;
        GLFrameBufferObject*    mStageIn;
        GLFrameBufferObject*    mStageIn2;
        GLFrameBufferObject*    mStageOut;
        GLFrameBufferObject     mRGB32FBuffer1;
        GLFrameBufferObject     mRGB32FBuffer2;

        LinkedList<GLRenderNode> mRenderNodeList;

        GLFWwindow* mWindowHandle;

        PipelineStage* mShadowMapStage;
        PipelineStage* mPhongShadowStage;
        PipelineStage* mPhongModelStage;
        PipelineStage* mToneMapStage;
        PipelineStage* mGaussianBloomStage;
        PipelineStage* mScreenRenderStage;

        CString mName;
        CString mRenderName;
        CString mShadingLanguage;
    };

} // namespace Berserk

#endif //BERSERK_GLRENDERSYSTEM_H