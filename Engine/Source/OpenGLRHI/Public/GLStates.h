//
// Created by Egor Orachyov on 2019-08-23.
//

#ifndef BERSERK_GLSTATES_H
#define BERSERK_GLSTATES_H

namespace Berserk
{

    class GLDepthState final : public RHIDepthTestState
    {
    public:

        GLDepthState(bool writeMask, ECompareFunc compareFunc)
        {
            mWriteMask = writeMask;
            mCompareFunc = GLEnums::CompareFunc(compareFunc);
        }

        ~GLDepthState() override
        {

        }

        void enable() override
        {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(mCompareFunc);

            if (mWriteMask)
            {
                glDepthMask(GL_TRUE);
            }
            else
            {
                glDepthMask(GL_FALSE);
            }
        }

    private:

        GLboolean mWriteMask;
        GLenum mCompareFunc;

    };

    class GLBlendState final : public RHIBlendState
    {
    public:

        GLBlendState(EBlendFunc source,
                     EBlendFunc destination)
        {
            mBlendFuncSource = GLEnums::BlendFunc(source);
            mBlendFuncDestination = GLEnums::BlendFunc(destination);
        }

        ~GLBlendState() override
        {

        }

        void enable() override
        {
            glEnable(GL_BLEND);
            glBlendFunc(mBlendFuncSource, mBlendFuncDestination);
        }

    private:

        GLenum mBlendFuncSource;
        GLenum mBlendFuncDestination;

    };

    class GLFaceCullingState final : public RHIFaceCullingState
    {
    public:

        GLFaceCullingState(EFaceCulling face,
                           ERasterCullMode order)
        {
            mFace = GLEnums::FaceCulling(face);
            mOrder = GLEnums::RasterCullMode(order);
        }

        ~GLFaceCullingState() override
        {

        }

        void enable() override
        {
            glEnable(GL_CULL_FACE);
            glCullFace(mFace);
            glFrontFace(mOrder);
        }

    private:

        GLenum mFace;
        GLenum mOrder;

    };

    class GLStencilState final : public RHIStencilTestState
    {
    public:

        GLStencilState(uint32 mask,
                       uint32 clear,
                       ECompareFunc compare,
                       uint32 value,
                       uint32 read,
                       EStencilOperation st_fail,
                       EStencilOperation dt_fail,
                       EStencilOperation dt_pass)
        {
            mClear = clear;
            mMask = mask;
            mCompareFunc = GLEnums::CompareFunc(compare);
            mValueRef = value;
            mReadMask = read;
            mSfail = GLEnums::StencilOperation(st_fail);
            mZfail = GLEnums::StencilOperation(dt_fail);
            mZpass = GLEnums::StencilOperation(dt_pass);
        }

        ~GLStencilState() override
        {

        }

        void enable() override
        {
            glEnable(GL_STENCIL_TEST);
            glClearStencil(mClear);
            glStencilMask(mMask);
            glStencilFunc(mCompareFunc, mValueRef, mReadMask);
            glStencilOp(mSfail, mZfail, mZpass);
        }

    private:

        GLint mClear;
        GLuint mMask;
        GLenum mCompareFunc;
        GLint mValueRef;
        GLuint mReadMask;
        GLenum mSfail;
        GLenum mZfail;
        GLenum mZpass;

    };

} // namespace Berserk

#endif //BERSERK_GLSTATES_H
