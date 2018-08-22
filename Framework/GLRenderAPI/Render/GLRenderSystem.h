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
#include "Buffers/GLGBuffer.h"
#include "Buffers/GLSSAOBuffer.h"
#include "Buffers/GLDepthMap.h"
#include "Buffers/GLCubeDepthMap.h"

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
        void setShadowQuality(ShadowInfo quality) override;
        void setSSAOBufferSize(FLOAT32 partOfScreen) override;
        void setSSAORadius(FLOAT32 radius) override;
        void setWindowName(const CStaticString& name) override;

        void enableToneMap(bool setIn) override;
        void enableGaussianBloom(bool setIn) override;
        void enableSSAO(bool setIn) override;

        bool isEnabledToneMap() override;
        bool isEnabledGaussianBloom() override;
        bool isEnabledSSAO() override;

        Camera* getRenderCamera() override;
        const Vector3f& getAmbientLightSource() const override;
        const Vector4f& getClearColor() const override;
        ShadowInfo getShadowQuality() const override;
        UINT32 getShadowMapSize() const override;
        FLOAT32 getSSAOBufferSize() override;
        FLOAT32 getSSAORadius() override;
        const CStaticString& getWindowName() const override;

        void setExposure(FLOAT32 exposure) override;
        void setLuminanceThresh(FLOAT32 luminance) override;
        void setGammaCorrection(FLOAT32 gamma) override;

        FLOAT32 getExposure() const override;
        FLOAT32 getLuminanceThresh() const override;
        FLOAT32 getGammaCorrection() const override;

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

        bool isReSized() override;

        void queueShadowLightSource(SpotLight* light) override;
        void queueShadowLightSource(PointLight* light) override;
        void queueShadowLightSource(DirectionalLight* light) override;
        void queueLightSource(SpotLight* light) override;
        void queueLightSource(PointLight* light) override;
        void queueLightSource(DirectionalLight* light) override;
        void queueRenderNode(RenderNode* node) override;

        List<SpotLight*>        &getSpotShadowSources() override;
        List<PointLight*>       &getPointShadowSources() override;
        List<DirectionalLight*> &getDirectionalShadowSources() override;
        List<SpotLight*>        &getSpotLightSources() override;
        List<PointLight*>       &getPointLightSources() override;
        List<DirectionalLight*> &getDirectionalLightSources() override;
        List<RenderNode*>       &getRenderNodeSources() override;

        GBuffer*        getGBuffer() override;
        SSAOBuffer*     getSSAOBuffer() override;
        DepthMap*       getDirDepthMaps() override;
        DepthMap*       getSpotDepthMaps() override;
        CubeDepthMap*   getPointDepthMaps() override;

        RenderNode* createRenderNode() override;
        void deleteRenderNode(RenderNode* node) override;

        TextureManager &getTextureManagerRef() override;
        TextureManager *getTextureManagerPtr() override;

        MaterialManager &getMaterialManagerRef() override;
        MaterialManager *getMaterialManagerPtr() override;

        RenderMeshManager &getRenderMeshManagerRef() override;
        RenderMeshManager *getRenderMeshManagerPtr() override;

        GLScreenPlane* getScreenPlane();
        GLFrameBufferObject* getStageOutBuffer();
        GLFrameBufferObject* getStageInBuffer();

    private:

        void printContextInfo() const;
        void getContextInfo();
        void setUpShadowMaps(ShadowInfo quality);
        void swap();

    private:

        INT32 mWindowWidth;
        INT32 mWindowHeight;
        INT32 mPixelWindowWidth;
        INT32 mPixelWindowHeight;
        INT32 mOldPixelWindowWidth;
        INT32 mOldPixelWindowHeight;
        INT32 mWindowPosX;
        INT32 mWindowPosY;

        INT8 mIsReSized         : 1;
        INT8 mUseToneMap        : 1;
        INT8 mUseGaussianBloom  : 1;
        INT8 mUseSSAO           : 1;

        Vector3f mAmbientLight;
        Vector4f mClearColor;
        FLOAT32  mLuminanceThresh;
        FLOAT32  mExposure;
        FLOAT32  mGammaCorrection;

        UINT32      mShadowMapSize;
        ShadowInfo  mShadowQuality;

        FLOAT32 mSSAOScreenBufferPart;
        FLOAT32 mSSAORadius;

        Camera*                 mRenderCamera;
        List<SpotLight*>        mSpotShadowSources;
        List<PointLight*>       mPointShadowSources;
        List<DirectionalLight*> mDirectionalShadowSources;
        List<SpotLight*>        mSpotLightSources;
        List<PointLight*>       mPointLightSources;
        List<DirectionalLight*> mDirectionalLightSources;
        List<RenderNode*>       mRenderNodeSources;

        GLGBuffer mGBuffer;
        GLSSAOBuffer mSSAOBuffer;
        GLDepthMap mSpotDepthMap[ShadowInfo::SI_MAX_SPOT_SHADOW_SOURCES];
        GLDepthMap mDirectionalDepthMap[ShadowInfo::SI_MAX_DIR_SHADOW_SOURCES];
        GLCubeDepthMap mPointDepthMap[ShadowInfo::SI_MAX_POINT_SHADOW_SOURCES];

        GLTextureManager    mTextureManager;
        GLMaterialManager   mMaterialManager;
        GLRenderMeshManager mRenderMeshManager;


        GLScreenPlane           mScreenPlane;
        GLFrameBufferObject*    mStageIn;
        GLFrameBufferObject*    mStageOut;
        GLFrameBufferObject     mRGB32FBuffer1;
        GLFrameBufferObject     mRGB32FBuffer2;

        LinkedList<GLRenderNode> mRenderNodeList;

        GLFWwindow* mWindowHandle;

        PipelineStage* mDeferredStage;
        PipelineStage* mShadowMapStage;
        PipelineStage* mAmbientOcclusionStage;
        PipelineStage* mDeferredPhongShadowStage;
        PipelineStage* mPhongShadowStage;
        PipelineStage* mPhongModelStage;
        PipelineStage* mToneMapStage;
        PipelineStage* mGaussianBloomStage;
        PipelineStage* mScreenRenderStage;

        CString mWindowName;
        CString mName;
        CString mRenderName;
        CString mShadingLanguage;
    };

} // namespace Berserk

#endif //BERSERK_GLRENDERSYSTEM_H