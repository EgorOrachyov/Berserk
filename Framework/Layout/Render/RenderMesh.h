//
// Created by Egor Orachyov on 08.08.2018.
//

#ifndef BERSERKENGINE_RENDERMESH_H
#define BERSERKENGINE_RENDERMESH_H

#include "GPUBuffer.h"
#include "RenderResource.h"
#include "Containers/ArrayList.h"
#include "Components/VertexComponents.h"

namespace Berserk
{
    enum MeshType
    {
        MT_NO_TYPE = 0x0000,
        MT_POSITION = 0x0001,
        MT_NORMAL = 0x0002,
        MT_BITANGENT = 0x0004,
        MT_TANGENT = 0x0008,
        MT_TEXCOORDS = 0x0010,

        MT_P = MT_POSITION,
        MT_PN = MT_POSITION | MT_NORMAL,
        MT_PT = MT_POSITION | MT_TEXCOORDS,
        MT_PNT = MT_PN | MT_TEXCOORDS,
        MT_PNBTT = MT_PNT | MT_BITANGENT | MT_TANGENT
    };

    class RenderMesh : public RenderResource
    {
    public:

        virtual ~RenderMesh() = default;

        virtual void destroy() = 0;

        virtual void setType(UINT32 mask) = 0;
        virtual UINT32 getType() = 0;

        virtual void addGeometryInfo(Vertex* data, UINT32 count) = 0;
        virtual void addGeometryInfo(VertexPN* data, UINT32 count) = 0;
        virtual void addGeometryInfo(VertexPT* data, UINT32 count) = 0;
        virtual void addGeometryInfo(VertexPNT* data, UINT32 count) = 0;
        virtual void addGeometryInfo(VertexPNBTT* data, UINT32 count) = 0;

        virtual void addGeometryInfo(UINT16* indices, UINT32 count) = 0;
        virtual void addGeometryInfo(UINT32* indices, UINT32 count) = 0;

        virtual void addGeometryInfo(ArrayList<Vertex> &data) = 0;
        virtual void addGeometryInfo(ArrayList<VertexPN> &data) = 0;
        virtual void addGeometryInfo(ArrayList<VertexPT> &data) = 0;
        virtual void addGeometryInfo(ArrayList<VertexPNT> &data) = 0;
        virtual void addGeometryInfo(ArrayList<VertexPNBTT> &data) = 0;

        virtual void addGeometryInfo(ArrayList<UINT16> &indices) = 0;
        virtual void addGeometryInfo(ArrayList<UINT32> &indices) = 0;

        virtual GPUBuffer& getGPUBuffer() = 0;
    };

} // namespace Berserk

#endif //BERSERKENGINE_RENDERMESH_H