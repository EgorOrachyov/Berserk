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
                eVT_Position    = SHIFT(0),
                eVT_Normal      = SHIFT(1),
                eVT_Tangent     = SHIFT(2),
                eVT_Bitangent   = SHIFT(3),
                eVT_TexCoords   = SHIFT(4),

                eVT_Vertex      = eVT_Position,
                eVT_VertexPN    = eVT_Position  | eVT_Normal,
                eVT_VertexPT    = eVT_Position  | eVT_TexCoords,
                eVT_VertexPNT   = eVT_VertexPN  | eVT_TexCoords,
                eVT_VertexPNTBT = eVT_VertexPNT | eVT_Tangent    | eVT_Bitangent
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
             * Creates gpu vertex array object with chosen params and
             * data of the mesh/model object
             * @param verticesCount Count of vertices in the array
             * @param vertexType    Type of vertices in the array
             * @param vertices      Pointer to data
             * @param indicesCount  Count of indices
             * @param indices       Pointer to indices
             * @param usage         Buffer usage - say VRAM how we will use/update our buffer
             */
            virtual void create(uint32 verticesCount,
                                VertexType vertexType,
                                void* vertices,
                                uint32 indicesCount,
                                uint16* indices,
                                IRenderDriver::BufferUsage usage) = 0;

            /** Update buffer with data */
            virtual void update(uint32 size, const void *data) = 0;

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

            /** Send that buffer data to the gpu to render it only for first indicesCount */
            virtual void draw(uint32 indicesCount) = 0;

            /** @return Type of vertices in the buffer */
            virtual VertexType getVertexType() = 0;

            /** @return Count of vertices in the buffer */
            virtual uint32 getVertexCount() = 0;

            /** @return Memory used at gpu side */
            virtual uint32 getGPUMemoryUsage() = 0;

        };

    } // namespace Resources

} // namespace Berserk

#endif //BERSERK_IGPUBUFFER_H