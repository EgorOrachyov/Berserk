//
// Created by Egor Orachyov on 04.05.2019.
//

#ifndef BERSERK_GEOMETRYBUFFERS_H
#define BERSERK_GEOMETRYBUFFERS_H

#include <Foundation/RenderBase.h>

namespace Berserk::Render
{

    /**
     * Allows to create common geometry buffer for rendering primitives
     * (Primary for debug purposes)
     */
    class ENGINE_API GeometryBuffers
    {
    public:

        /**
         * Create buffer for box with center in (0,0,0) with specified type and size
         * @param name Name of the buffer to find/name
         * @param type Type of vertices for the mesh
         * @param width Size in the x space
         * @param height Size in the y space
         * @param depth Size in the z space
         * @return Pointer to the created and setup buffer
         */
        static IGPUBuffer* createBox(const char* name,
                                     Resources::IGPUBuffer::VertexType type,
                                     float32 width, float32 height, float32 depth);

        /**
         * Create buffer for cube with center in (0,0,0) with specified type and size
         * @param name Name of the buffer to find/name
         * @param type Type of vertices for the mesh
         * @param size Size for the box in all x,y,z spaces
         * @return Pointer to the created and setup buffer
         */
        static IGPUBuffer* createCube(const char* name,
                                      Resources::IGPUBuffer::VertexType type,
                                      float32 size);

        /**
         * Create buffer for plane of 0 height with center in (0,0,0) with specified type and size
         * @param name Name of the buffer to find/name
         * @param type Type of vertices for the mesh
         * @param width Size in the x space
         * @param depth Size in the z space
         * @return Pointer to the created and setup buffer
         */
        static IGPUBuffer* createPlane(const char *name,
                                       Resources::IGPUBuffer::VertexType type,
                                       float32 width, float32 depth);

        /**
         * Create buffer for screen in oXY space with center in (0,0,0) of VertexPT type
         * @param name Name of the buffer to find/name
         * @param width Size in the x space
         * @param height Size in the y space
         * @return Pointer to the created and setup buffer
         */
        static IGPUBuffer* createScreen(const char* name,
                                       float32 width, float32 height);

    };

} // namespace Berserk::Render

#endif //BERSERK_GEOMETRYBUFFERS_H