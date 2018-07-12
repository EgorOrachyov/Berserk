//
// Created by Egor Orachyov on 04.07.2018.
//

#include <Strings/WString.h>
#include "GLRenderSystem.h"
#include "Logging/LogMessages.h"
#include "Memory/MemoryAllocators.h"

namespace Berserk
{

    GLRenderSystem::GLRenderSystem()
    {
        mName = CString("Open GL Rendering System");
        mRenderName = CString("Open GL Graphic driver 4.1");
        mShadingLanguage = CString("GLSL 4.1");
    }

    GLRenderSystem::~GLRenderSystem()
    {
        PUSH("Delete GL Render System %p\n", this);

        destroy();
    }

    void GLRenderSystem::init()
    {
        PUSH("Init GL Render System %p\n", this);

        // todo: add config table

        mContext.initWindowContext();
        mWindow.create();
        mWindow.makeCurrent();
        mContext.initRenderingContext();
    }

    void GLRenderSystem::destroy()
    {

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

} // namespace Berserk