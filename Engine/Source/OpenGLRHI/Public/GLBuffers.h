//
// Created by Egor Orachyov on 2019-08-23.
//

#ifndef BERSERK_GLBUFFERS_H
#define BERSERK_GLBUFFERS_H

namespace Berserk
{

    template <typename RHIBaseType, GLenum type>
    class GLBufferBase : public RHIBaseType
    {
    public:

        GLBufferBase(GLenum bufferUsage, uint32 size, const uint8* data);

        ~GLBufferBase() override;

        void update(uint32 size, const uint8 *data) override;

        uint32 getSize() const override;

        uint32 getMemoryUsage() const override;

        uint32 getMemoryUsage_GPU() const override;

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
    uint32 GLBufferBase<RHIBaseType, type>::getMemoryUsage() const
    {
        return sizeof(GLBufferBase<RHIBaseType, type>);
    }

    template<typename RHIBaseType, GLenum type>
    uint32 GLBufferBase<RHIBaseType, type>::getMemoryUsage_GPU() const
    {
        return mBufferSize;
    }

    typedef GLBufferBase<RHIVertexBuffer, GL_ARRAY_BUFFER> GLVertexBuffer;


    class GLIndexBuffer : public GLBufferBase<RHIIndexBuffer, GL_ELEMENT_ARRAY_BUFFER>
    {
    public:

        GLIndexBuffer(GLenum bufferUsage, uint32 size, const uint8* data, GLenum indexType)
                : GLBufferBase(bufferUsage, size, data), mIndicesType(indexType)
        {

        }

        GLenum getIndicesType() const { return mIndicesType; }

        uint32 getMemoryUsage() const override { return sizeof(GLIndexBuffer); }

    private:

        GLenum mIndicesType;

    };

    class GLIndexedGeometryBuffer final : public RHIGeometryBuffer
    {
    public:

        GLIndexedGeometryBuffer(const RHIVertexBufferRef &vertexBuffer, const RHIIndexBufferRef &indexBuffer,
                                EDataLayout dataLayout, EPrimitiveType primitiveType,
                                GLenum primitiveType_gl, GLenum indexType_gl);

        ~GLIndexedGeometryBuffer() override;

        void draw(uint32 numOfElements) override;

        void draw(uint32 numOfIndices, uint32 offsetBuffer) override;

        EPrimitiveType getPrimitiveType() const override;

        uint32 getMemoryUsage() const override;

        uint32 getMemoryUsage_GPU() const override;

    private:

        EPrimitiveType mPrimitiveType;

        GLenum mGL_primitiveType;
        GLenum mGL_indicesType;
        GLuint mResourceID = 0;

        RHIVertexBufferRef mVertexBuffer;
        RHIIndexBufferRef mIndexBuffer;

    };

} // namespace Berserk

#endif //BERSERK_GLBUFFERS_H
