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
        ~GLRenderSystem() override;

        void init(const ConfigTable& table) override;
        void destroy() override;
        void validate() override;

        void preMainLoop() override;
        void preUpdate() override;
        void postUpdate() override;
        void postMainLoop() override;

        const CString& getName() const override;
        const CString& getRenderName() const override;
        const CString& getShadingLanguageName() const override;

    private:

        void printContextInfo() const;

    protected:

        CString mName;
        CString mRenderName;
        CString mShadingLanguage;

        GLWindow  mWindow;

        GLRenderPipeline* mPipeline;

    };

} // namespace Berserk

#endif //BERSERK_GLRENDERSYSTEM_H