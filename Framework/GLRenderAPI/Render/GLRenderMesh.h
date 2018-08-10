//
// Created by Egor Orachyov on 10.08.2018.
//

#ifndef BERSERKENGINE_GLRENDERMESH_H
#define BERSERKENGINE_GLRENDERMESH_H

#include "Render/RenderMesh.h"
#include "Buffers/GLGPUBuffer.h"

namespace Berserk
{

    class GLRenderMesh : public RenderMesh
    {
    public:

        GLRenderMesh();
        virtual ~GLRenderMesh() = default;

        void destroy() override;

        UINT32 getMemoryUsage() const override;

        void   addReference() override;
        void   release() override;
        UINT32 getReferences() const override;

        void setName(const CStaticString& name) override;
        const CStaticString& getName() const override;

        void setType(UINT32 mask) override;
        UINT32 getType() override;

        void addGeometryInfo(Vertex* data, UINT32 count) override;
        void addGeometryInfo(VertexPN* data, UINT32 count) override;
        void addGeometryInfo(VertexPT* data, UINT32 count) override;
        void addGeometryInfo(VertexPNT* data, UINT32 count) override;
        void addGeometryInfo(VertexPNBTT* data, UINT32 count) override;

        void addGeometryInfo(UINT16* indices, UINT32 count) override;
        void addGeometryInfo(UINT32* indices, UINT32 count) override;

        void addGeometryInfo(ArrayList<Vertex> &data) override;
        void addGeometryInfo(ArrayList<VertexPN> &data) override;
        void addGeometryInfo(ArrayList<VertexPT> &data) override;
        void addGeometryInfo(ArrayList<VertexPNT> &data) override;
        void addGeometryInfo(ArrayList<VertexPNBTT> &data) override;

        void addGeometryInfo(ArrayList<UINT16> &indices) override;
        void addGeometryInfo(ArrayList<UINT32> &indices) override;

        GPUBuffer& getGPUBuffer() override;
        GLGPUBuffer& getGLGPUBuffer();

        const bool operator == (const GLRenderMesh& mesh) const;

    private:

        UINT32 mReferenceCount;
        UINT32 mType;
        GLGPUBuffer mGPUBuffer;

        CStaticString mName;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLRENDERMESH_H