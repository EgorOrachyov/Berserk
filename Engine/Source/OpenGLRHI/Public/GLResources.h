//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_GLRESOURCES_H
#define BERSERK_GLRESOURCES_H

#include <RHIResources.h>
#include <RHIDefinitions.h>
#include <GLCommon.h>
#include <Math/MathInclude.h>

namespace Berserk
{

    class GLUniformBuffer final : public RHIUniformBuffer
    {
    public:

        GLUniformBuffer(uint32 bindingPoint, GLenum bufferUsage, uint32 size, const uint8* data);

        ~GLUniformBuffer() override;

        void update(uint32 size, const uint8 *data) override;

        void update(uint32 size, uint32 offset, const uint8 *data) override;

        void bind(uint32 bindingPoint) const override;

        void bind() const override;

        void setBindingPoint(uint32 bindingPoint) override;

        uint32 getBindingPoint() const override;

        uint32 getSize() const override;

        uint32 getMemoryUsage() override;

        uint32 getMemoryUsage_GPU() override;

    private:

        GLuint mResourceID = 0;
        uint32 mBufferSize = 0;
        uint32 mBindingPoint = 0;

    };

    template <typename RHIBaseType, GLenum type>
    class GLBufferBase final : public RHIBaseType
    {
    public:

        GLBufferBase(GLenum bufferUsage, uint32 size, const uint8* data);

        ~GLBufferBase() override;

        void update(uint32 size, const uint8 *data) override;

        uint32 getSize() const override;

        uint32 getMemoryUsage() override;

        uint32 getMemoryUsage_GPU() override;

    private:

        friend class GLIndexedGeometryBuffer;

        GLuint mResourceID = 0;
        uint32 mBufferSize = 0;

    };

    template<typename RHIBaseType, GLenum type>
    GLBufferBase<RHIBaseType, type>::GLBufferBase(GLenum bufferUsage, Berserk::uint32 size,
            const Berserk::uint8 *data) : mBufferSize(size)
    {
        glGenBuffers(1, &mResourceID);
        glBindBuffer(type, mResourceID);
        glBufferData(type, size, data, bufferUsage);
    }

    template<typename RHIBaseType, GLenum type>
    GLBufferBase<RHIBaseType, type>::~GLBufferBase()
    {
        if (mResourceID)
        {
            glDeleteBuffers(1, &mResourceID);
            mResourceID = 0;
        }
    }

    template<typename RHIBaseType, GLenum type>
    void GLBufferBase<RHIBaseType, type>::update(uint32 size, const uint8 *data)
    {
        glBindBuffer(type, mResourceID);
        glBufferSubData(type, 0 , size, data);
        glBindBuffer(type, 0);
    }

    template<typename RHIBaseType, GLenum type>
    uint32 GLBufferBase<RHIBaseType, type>::getSize() const
    {
        return mBufferSize;
    }

    template<typename RHIBaseType, GLenum type>
    uint32 GLBufferBase<RHIBaseType, type>::getMemoryUsage()
    {
        return sizeof(GLBufferBase<RHIBaseType, type>);
    }

    template<typename RHIBaseType, GLenum type>
    uint32 GLBufferBase<RHIBaseType, type>::getMemoryUsage_GPU()
    {
        return mBufferSize;
    }

    typedef GLBufferBase<RHIVertexBuffer, GL_ARRAY_BUFFER> GLVertexBuffer;
    typedef GLBufferBase<RHIIndexBuffer, GL_ELEMENT_ARRAY_BUFFER> GLIndexBuffer;

    class GLSampler final : public RHISampler
    {
    public:

        GLSampler(ESamplerFilter min, ESamplerFilter mag, ESamplerWrapMode wrapMode,
                const Vec4f &borderColor, GLint min_gl, GLint mag_gl, GLint wrapMode_gl);

        ~GLSampler() override;

        void bind(uint32 textureSlot) override;

        ESamplerFilter getMinFilter() const override;

        ESamplerFilter getMagFilter() const override;

        ESamplerWrapMode getWrapMode() const override;

        const Vec4f &getBorderColor() const override;

    private:

        GLuint mResourceID = 0;
        ESamplerFilter mMinFilter;
        ESamplerFilter mMagFilter;
        ESamplerWrapMode mWrapMode;
        Vec4f mBorderColor;

    };

    class GLIndexedGeometryBuffer final : public RHIGeometryBuffer
    {
    public:

        GLIndexedGeometryBuffer(const RHIVertexBufferRef &vertexBuffer, const RHIIndexBufferRef &indexBuffer,
                                EDataLayout dataLayout, EPrimitiveType primitiveType, uint32 indicesCount,
                                GLenum primitiveType_gl, GLenum indexType_gl);

        ~GLIndexedGeometryBuffer() override;

        void draw() override;

        void draw(uint32 numOfElements) override;

        EPrimitiveType getPrimitiveType() const override;

        uint32 getMemoryUsage() override;

        uint32 getMemoryUsage_GPU() override;

    private:

        EPrimitiveType mPrimitiveType;

        GLenum mGL_primitiveType;
        GLenum mGL_indicesType;
        GLuint mResourceID = 0;
        uint32 mIndicesCount = 0;

        RHIVertexBufferRef mVertexBuffer;
        RHIIndexBufferRef mIndexBuffer;

    };

} // namespace Berserk

#endif //BERSERK_GLRESOURCES_H