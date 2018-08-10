//
// Created by Egor Orachyov on 10.08.2018.
//

#include "Render/GLRenderDriver.h"
#include "Essential/GLInclude.h"

namespace Berserk
{

    void GLRenderDriver::init(const ConfigTable& table)
    {

    }

    void GLRenderDriver::destroy()
    {

    }

    void GLRenderDriver::clearColorBuffer(const Vector3f &color)
    {
        glClearColor(color.x, color.y, color.z, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GLRenderDriver::clearColorBuffer(const Vector4f &color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GLRenderDriver::clearBuffer()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void GLRenderDriver::clearColorBuffer()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GLRenderDriver::clearDepthBuffer()
    {
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void GLRenderDriver::clearStencilBuffer()
    {
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    void GLRenderDriver::setDefaultBuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLRenderDriver::setClearColor(const Vector3f &color)
    {
        glClearColor(color.x, color.y, color.z, 0.0);
    }

    void GLRenderDriver::setClearColor(const Vector4f &color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
    }
    
    void GLRenderDriver::setWindingOrderCW()
    {
        glFrontFace(GL_CW);
    }

    void GLRenderDriver::setWindingOrderCCW()
    {
        glFrontFace(GL_CCW);
    }

    void GLRenderDriver::setFrontCulling()
    {
        glCullFace(GL_FRONT);
    }

    void GLRenderDriver::setBackCulling()
    {
        glCullFace(GL_BACK);
    }

    void GLRenderDriver::enableDepthTest(bool flag)
    {
        if (flag) glEnable(GL_DEPTH_TEST);
        else glDisable(GL_DEPTH_TEST);
    }

    void GLRenderDriver::enableFaceCulling(bool flag)
    {
        if (flag) glEnable(GL_CULL_FACE);
        else glDisable(GL_CULL_FACE);
    }

} // namespace Berserk