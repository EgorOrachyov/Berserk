//
// Created by Egor Orachyov on 10.08.2018.
//

#include "Render/GLRenderMesh.h"

namespace Berserk
{

    GLRenderMesh::GLRenderMesh()
    {
        mType = MeshType::MT_NO_TYPE;
        mReferenceCount = 0;
    }

    void GLRenderMesh::destroy()
    {
        mGPUBuffer.destroy();
    }

    UINT32 GLRenderMesh::getMemoryUsage() const
    {
        return 0;
    }

    void   GLRenderMesh::addReference()
    {
        mReferenceCount += 1;
    }

    void   GLRenderMesh::release()
    {
        if (mReferenceCount > 0) mReferenceCount -= 1;
        if (mReferenceCount == 0) destroy();
    }
    UINT32 GLRenderMesh::getReferences() const
    {
        return mReferenceCount;
    }

    void GLRenderMesh::setName(const CStaticString& name)
    {
        mName = name;
    }

    const CStaticString& GLRenderMesh::getName() const
    {
        return mName;
    }

    void GLRenderMesh::setType(UINT32 mask)
    {
        mType = mask;
    }

    UINT32 GLRenderMesh::getType()
    {
        return mType;
    }

    void GLRenderMesh::addGeometryInfo(Vertex* data, UINT32 count)
    {
        if (mType == MeshType::MT_P)
        {
            mGPUBuffer.init();
            mGPUBuffer.attachData(data, count);
        }
    }

    void GLRenderMesh::addGeometryInfo(VertexPN* data, UINT32 count)
    {
        if (mType == MeshType::MT_PN)
        {
            mGPUBuffer.init();
            mGPUBuffer.attachData(data, count);
        }
    }

    void GLRenderMesh::addGeometryInfo(VertexPNT* data, UINT32 count)
    {
        if (mType == MeshType::MT_PNT)
        {
            mGPUBuffer.init();
            mGPUBuffer.attachData(data, count);
        }
    }

    void GLRenderMesh::addGeometryInfo(VertexPNBTT* data, UINT32 count)
    {
        if (mType == MeshType::MT_PNBTT)
        {
            mGPUBuffer.init();
            mGPUBuffer.attachData(data, count);
        }
    }

    void GLRenderMesh::addGeometryInfo(UINT16 *indices, UINT32 count)
    {
        mGPUBuffer.attachIndices(indices, count);
    }

    void GLRenderMesh::addGeometryInfo(UINT32 *indices, UINT32 count)
    {
        mGPUBuffer.attachIndices(indices, count);
    }

    void GLRenderMesh::addGeometryInfo(ArrayList<Vertex> &data)
    {
        if (mType == MeshType::MT_P)
        {
            mGPUBuffer.init();
            mGPUBuffer.attachData(data);
        }
    }

    void GLRenderMesh::addGeometryInfo(ArrayList<VertexPN> &data)
    {
        if (mType == MeshType::MT_PN)
        {
            mGPUBuffer.init();
            mGPUBuffer.attachData(data);
        }
    }

    void GLRenderMesh::addGeometryInfo(ArrayList<VertexPNT> &data)
    {
        if (mType == MeshType::MT_PNT)
        {
            mGPUBuffer.init();
            mGPUBuffer.attachData(data);
        }
    }

    void GLRenderMesh::addGeometryInfo(ArrayList<VertexPNBTT> &data)
    {
        if (mType == MeshType::MT_PNBTT)
        {
            mGPUBuffer.init();
            mGPUBuffer.attachData(data);
        }
    }

    void GLRenderMesh::addGeometryInfo(ArrayList<UINT16> &indices)
    {
        mGPUBuffer.attachIndices(indices);
    }

    void GLRenderMesh::addGeometryInfo(ArrayList<UINT32> &indices)
    {
        mGPUBuffer.attachIndices(indices);
    }

    GPUBuffer& GLRenderMesh::getGPUBuffer()
    {
        return mGPUBuffer;
    }

    GLGPUBuffer& GLRenderMesh::getGLGPUBuffer()
    {
        return mGPUBuffer;
    }

    const bool GLRenderMesh::operator == (const GLRenderMesh &mesh) const
    {
        return (mName == mesh.mName);
    }

} // namespace Berserk