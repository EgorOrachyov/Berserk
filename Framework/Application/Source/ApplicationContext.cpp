//
// Created by Egor Orachyov on 12.07.2018.
//

#include "ApplicationContext.h"
#include "GLRenderSystem.h"
#include "Essential/Common.h"

namespace Berserk
{

    void ApplicationContext::init()
    {
        mRenderSystem = new GLRenderSystem();

        mRenderSystem->init();
    }

    void ApplicationContext::setup()
    {

    }

    void ApplicationContext::run()
    {

    }

    void ApplicationContext::close()
    {

    }

    void ApplicationContext::destroy()
    {
        SAFE_DELETE(mRenderSystem);
    }

} // namespace Berserk