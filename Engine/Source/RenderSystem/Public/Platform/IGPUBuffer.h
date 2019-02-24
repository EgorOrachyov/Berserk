//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_IGPUBUFFER_H
#define BERSERK_IGPUBUFFER_H

#include "Misc/Bits.h"
#include "Misc/Types.h"
#include "Platform/ITexture.h"
#include "Resource/IResource.h"
#include "Misc/UsageDescriptors.h"
#include "Platform/IRenderDriver.h"

namespace Berserk
{

    class GRAPHICS_API IGPUBuffer : public IResource
    {
    public:

        enum VertexType : uint32
        {
            Position    = SHIFT(0),
            Normal      = SHIFT(1),
            Tangent     = SHIFT(2),
            Bitangent   = SHIFT(3),
            TexCoords   = SHIFT(4),

            Vertex      = Position,
            VertexPN    = Position | Normal,
            VertexPT    = Position | TexCoords,
            VertexPNT   = VertexPN | TexCoords,
            VertexPNTBT = VertexPNT | Tangent | Bitangent
        };

    public:

        virtual void create(uint32 verticesCount,
                            VertexType vertexType,
                            void* vertices,
                            uint32 indicesCount,
                            uint16* indices) = 0;

        virtual void setDrawingProperties(uint32 count,
                                          IRenderDriver::PrimitiveType primitiveType,
                                          uint32 indicesType) = 0;

        virtual void draw() = 0;

        virtual VertexType getVertexType() = 0;

        virtual uint32 getGPUMemoryUsage() = 0;

    };

} // namespace Berserk

#endif //BERSERK_IGPUBUFFER_H