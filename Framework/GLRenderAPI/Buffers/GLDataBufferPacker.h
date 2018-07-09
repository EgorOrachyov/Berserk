//
// Created by Egor Orachyov on 04.07.2018.
//

#ifndef BERSERKENGINE_GLDATABUFFERPACKER_H
#define BERSERKENGINE_GLDATABUFFERPACKER_H

#include "Essential/GLDataType.h"
#include "Essential/GLNormalization.h"

#include "Essential/Types.h"
#include "Containers/ArrayList.h"

#include "Math/Vector2f.h"
#include "Math/Vector3f.h"
#include "Math/Vector4f.h"

namespace Berserk
{

    /**
     * Data packer allows you to group separate buffers describes vertexes data to one buffer, pack it
     * as one array and then simply create VertexArrayObject to pass its data gpu shader program.
     *
     * @note How to use: if you have buffers with: vertex position for attribute location 0, vertex color for attribute location 1,
     *      vertex normal for attribute location 3 and etc. pass it to packer with info about data size, count and then pack it
     *      to pass to VertexArrayObject.
     */
    class DATA_API GLDataBufferPacker
    {
    private:

        /**
         * Metadata to share per vertex params in vao buffer
         */
        typedef struct VertexData
        {
        public:

            void* data;
            UINT32 size;
            UINT32 perVertCount;
            GLuint index;
            GLenum type;
            GLboolean normalized;
            const GLvoid* offset;

        } VertexData;

    public:

        GLDataBufferPacker();
        ~GLDataBufferPacker();

        /**
         * Initialize packer
         */
        void init();

        /**
         * Free internal buffer and set all variables to 0
         */
        void destroy();

        /**
         * Creates huge buffer and packs all the data to create an array, where
         * vertexes params are grouped per vertex. It allows use only on VBO buffer
         * and allows store data in GPU as a single buffer
         */
        void pack();

        /**
         * Free internal buffer and prepare for another packing
         */
        void reset();

        /**
         * Allows to add packer data about concrete vertex attribute for all the vertexes of the buffer
         *
         * @warning data pointer should be valid before pack() call
         * @warning packer do not check borders of buffers
         *
         *
         * @param data Pointer to array with data
         * @param size Size in bytes of one BLOCK of data (it means that if it is vec4f => size = 4 * sizeof(float32))
         * @param perVertexCount
         * @param count Number of BLOCKS (or number of vertexes which are described by this data)
         * @param attributeIndex Index of attribute from shader program (get by GLGPUProgram::getAttributeLocation)
         * @param type Type of the basic data of block (for block vec4f basic data type is GLPT_FLOAT)
         * @param usage Do you want to normalize data (transform to [0;1] or [-1;1])
         */
        void addVertexData(void *data, UINT32 size, UINT32 perVertexCount, UINT32 count, UINT32 attributeIndex,
                           GLDataType type, GLNormalization usage);

        /**
         * Allows to add packer data about concrete vertex vec2f attribute
         *
         * @warning data pointer should be valid before pack() call
         * @warning packer do not check borders of buffers
         *
         * @param data Pointer to array of vectors
         * @param count Number of described vertexes
         * @param attributeIndex Index of the attribute in the shader program (get by GLGPUProgram::getAttributeLocation)
         * @param usage Do you want to normalize data (transform to [0;1] or [-1;1])
         */
        void addVertexData(Vector2f* data, UINT32 count, UINT32 attributeIndex, GLNormalization usage);

        /**
         * Allows to add packer data about concrete vertex vec3f attribute
         *
         * @warning data pointer should be valid before pack() call
         * @warning packer do not check borders of buffers
         *
         * @param data Pointer to array of vectors
         * @param count Number of described vertexes
         * @param attributeIndex Index of the attribute in the shader program (get by GLGPUProgram::getAttributeLocation)
         * @param usage Do you want to normalize data (transform to [0;1] or [-1;1])
         */
        void addVertexData(Vector3f* data, UINT32 count, UINT32 attributeIndex, GLNormalization usage);

        /**
         * Allows to add packer data about concrete vertex vec4f attribute
         *
         * @warning data pointer should be valid before pack() call
         * @warning packer do not check borders of buffers
         *
         * @param data Pointer to array of vectors
         * @param count Number of described vertexes
         * @param attributeIndex Index of the attribute in the shader program (get by GLGPUProgram::getAttributeLocation)
         * @param usage Do you want to normalize data (transform to [0;1] or [-1;1])
         */
        void addVertexData(Vector4f* data, UINT32 count, UINT32 attributeIndex, GLNormalization usage);

        /**
         * @return Return true if data was successfully packed
         */
        bool isPacked() const;

        /**
         * @return Is packer initialized
         */
        bool isInitialized() const;

        /**
         * Get size of one (!ONE!) per vertex data block
         *
         * @note Size of buffer can be evaluated as Stride * count (count of vertexes)
         *
         * @return Stride in bytes
         */
        UINT32 getStride() const;

        /**
         * Get count of described vertexes in the buffer
         *
         * @return Count of vertexes
         */
        UINT32 getCount() const;

        /**
         * Get count of added buffers via add functions (or count of vertex attributes)
         *
         * @return Count of buffers
         */
        UINT32 getBuffersCount() const;

        /**
         * Total buffer size
         *
         * @note Size of buffer can be evaluated as Stride * count (count of vertexes)
         *
         * @return Size in bytes
         */
        UINT32 getTotalBufferSize() const;

        /**
         * Internal buffer with data
         *
         * @warning Only for Vertex Array Object
         *
         * @return Pointer to buffer
         */
        const void* getBuffer() const;

    protected:

        friend class GLGPUBuffer;

        void* mBuffer;                      // Internal buffer pointer (result packed data buffer)
        INT8  mIsInitialized;               // Initialization status
        INT8  mIsPacked;                    // Packing status
        UINT32 mCount;                      // Count of the described vertexes in the buffer
        UINT32 mStride;                     // Number of bytes to describe ONE vertex attributes
        UINT32 mTotalSize;                  // Total size of internal buffer in bytes
        ArrayList<VertexData> mMetaData;    // List with data about each vertex attribute

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLDATABUFFERPACKER_H