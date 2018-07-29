//
// Created by Egor Orachyov on 04.07.2018.
//

#ifndef BERSERK_GLRENDERSYSTEM_H
#define BERSERK_GLRENDERSYSTEM_H

#include "Pipeline/GLRenderPipeline.h"
#include "System/RenderSystem.h"
#include "System/GLContext.h"
#include "System/GLWindow.h"

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

        void setViewportBorders(UINT32 cinematic) override;
        void setViewportBorders(UINT32 left, UINT32 right, UINT32 bottom, UINT32 top) override;

        UINT32 getWindowWidth() const override;
        UINT32 getWindowHeight() const override;

        GPUBuffer *createGPUBuffer(const CStaticString &name) override;

    private:

        void printContextInfo() const;
        void getContextInfo();

    protected:

        CString mName;
        CString mRenderName;
        CString mShadingLanguage;

        GLWindow mWindow;

        GLRenderPipeline* mPreProcess;
        GLRenderPipeline* mMainProcess;
        GLRenderPipeline* mPostProcess;
    };

} // namespace Berserk

#endif //BERSERK_GLRENDERSYSTEM_H