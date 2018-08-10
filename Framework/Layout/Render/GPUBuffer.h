//
// Created by Egor Orachyov on 27.07.2018.
//

#ifndef BERSERKENGINE_GPUBUFFER_H
#define BERSERKENGINE_GPUBUFFER_H

#include "Objects/Object.h"
#include "Containers/ArrayList.h"
#include "Components/VertexComponents.h"

namespace Berserk
{

    class GPUBuffer
    {
    public:

        virtual ~GPUBuffer() = default;

        virtual UINT32 getMemoryUsage() = 0;

        virtual void init() = 0;
        virtual void destroy() = 0;

        virtual void attachData(ArrayList<Vertex> &vertices) = 0;
        virtual void attachData(ArrayList<VertexPN> &vertices) = 0;
        virtual void attachData(ArrayList<VertexPNT> &vertices) = 0;
        virtual void attachData(ArrayList<VertexPNBTT> &vertices) = 0;

        virtual void attachData(Vertex* data, UINT32 count) = 0;
        virtual void attachData(VertexPN* data, UINT32 count) = 0;
        virtual void attachData(VertexPNT* data, UINT32 count) = 0;
        virtual void attachData(VertexPNBTT* data, UINT32 count) = 0;

        virtual void attachIndices(ArrayList<UINT16> &indices) = 0;
        virtual void attachIndices(ArrayList<UINT32> &indices) = 0;

        virtual void attachIndices(UINT16* indices, UINT32 count) = 0;
        virtual void attachIndices(UINT32* indices, UINT32 count) = 0;

        virtual void drawData() const = 0;
        virtual void drawIndices() const = 0;

        virtual bool validate() const = 0;
    };

} // namespace Berserk

#endif //BERSERKENGINE_GPUBUFFER_H