//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_IGPUBUFFER_H
#define BERSERK_IGPUBUFFER_H

#include "Misc/Bits.h"
#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"
#include "Resource/IResource.h"
#include "Platform/IRenderDriver.h"

namespace Berserk
{

    namespace Resources
    {

        /**
         * Abstract gpu buffer for storing mesh/model data
         */
        class GRAPHICS_API IGPUBuffer : public IResource
        {
        public:

            /**
             * Vertex types for mesh/model data
             */
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

            /**
             * Creates gpu vertex array object with chosen params and
             * data of the mesh/model object
             * @param verticesCount Count of vertices in the array
             * @param vertexType    Type of vertices in the array
             * @param vertices      Pointer to data
             * @param indicesCount  Count of indices
             * @param indices       Pointer to indices
             */
            virtual void create(uint32 verticesCount,
                                VertexType vertexType,
                                void* vertices,
                                uint32 indicesCount,
                                uint16* indices) = 0;

            /**
             * Draw with params of that buffer
             * @param count         Count of indices to be rendered
             * @param primitiveType Type of primitives, defined by indices
             * @param indicesType   Type of indices in the array
             */
            virtual void draw(uint32 count,
                              IRenderDriver::PrimitiveType primitiveType,
                              IRenderDriver::DataType indicesType) = 0;

            /** Send that buffer data to the gpu to render it */
            virtual void draw() = 0;

            /** @return Type of vertices in the buffer */
            virtual VertexType getVertexType() = 0;

            /** @return Memory used at gpu side */
            virtual uint32 getGPUMemoryUsage() = 0;

        };

    } // namespace Resources

} // namespace Berserk

#endif //BERSERK_IGPUBUFFER_H