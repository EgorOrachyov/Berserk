//
// Created by Egor Orachyov on 04.07.2018.
//

#ifndef BERSERK_GLRENDERSYSTEM_H
#define BERSERK_GLRENDERSYSTEM_H

#include "Pipeline/GLRenderPipeline.h"
#include "Render/RenderSystem.h"

#include "Managers/GLTextureManager.h"
#include "Managers/GLMaterialManager.h"

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

        void deleteLightSource(SpotLight* light) override;
        void deleteLightSource(PointLight* light) override;
        void deleteLightSource(DirectionalLight* light) override;

        Camera* getRenderCamera() override;
        AmbientLight* getAmbientLightSource() override;

        ArrayList<SpotLight*>& getSpotLightSources() override;
        ArrayList<PointLight*>& getPointLightSources() override;
        ArrayList<DirectionalLight*>& getDirectionalLightSources() override;

        GPUBuffer *createGPUBuffer(const CStaticString &name) override; // todo: add memory buffer

        TextureManager &getTextureManagerRef() override;
        TextureManager *getTextureManagerPtr() override;

        MaterialManager &getMaterialManagerRef() override;
        MaterialManager *getMaterialManagerPtr() override;

    private:

        void printContextInfo() const;
        void getContextInfo();

    private:

        INT32 mWindowWidth;
        INT32 mWindowHeight;
        INT32 mPixelWindowWidth;
        INT32 mPixelWindowHeight;
        INT32 mWindowPosX;
        INT32 mWindowPosY;

        Vector4f mClearColor;

        Camera* mRenderCamera;
        AmbientLight* mAmbientLight;
        ArrayList<SpotLight*> mSpotLightSources;
        ArrayList<PointLight*> mPointLightSources;
        ArrayList<DirectionalLight*> mDirectionalLightSources;

        GLTextureManager mTextureManager;
        GLMaterialManager mMaterialManager;

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