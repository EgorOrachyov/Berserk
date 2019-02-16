//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_IGPUBUFFER_H
#define BERSERK_IGPUBUFFER_H

#include "Public/Misc/Bits.h"
#include "Public/Misc/Types.h"
#include "Public/Platform/ITexture.h"
#include "Public/Resource/IResource.h"
#include "Public/Misc/UsageDescriptors.h"

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

        /**
         * Drawing mode to interpret vao buffers data
         * (defines how program should send to GPU data)
         */
        enum PrimitiveMode : uint32
        {
            TRIANGLES,
            LINES,
            LINE_STRIP,
            LINE_LOOP,
            POINTS
        };

    public:

        virtual void create(uint32 verticesCount,
                            VertexType vertexType,
                            void* vertices,
                            uint32 indicesCount,
                            uint16* indices) = 0;

        virtual void setDrawingProperties(uint32 count,
                                          PrimitiveMode mode,
                                          ITexture::PixelType indicesType) = 0;

        virtual void draw() = 0;

        virtual VertexType getVertexType() = 0;

        virtual PrimitiveMode getPrimitiveMode() = 0;

        virtual uint32 getGPUMemoryUsage() = 0;

    };

} // namespace Berserk

#endif //BERSERK_IGPUBUFFER_H