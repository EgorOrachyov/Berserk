//
// Created by Egor Orachyov on 04.07.2018.
//

#ifndef BERSERKENGINE_GLVERTEXBUFFER_H
#define BERSERKENGINE_GLVERTEXBUFFER_H

#include "GLInclude.h"

#include "../Core/Essential/Types.h"
#include "../Core/Containers/ArrayList.h"

#include "../Core/Math/Vector2f.h"
#include "../Core/Math/Vector3f.h"
#include "../Core/Math/Vector4f.h"

/**
 * Common names of basic types in gpu program
 */
enum GLParamType
{
    GLPT_SHORT  = GL_SHORT,
    GLPT_USHORT = GL_UNSIGNED_SHORT,
    GLPT_INT    = GL_INT,
    GLPT_UINT   = GL_UNSIGNED_INT,
    GLPT_FLOAT  = GL_FLOAT,
    GLPT_DOUBLE = GL_DOUBLE
};

/**
 * Flags to use or do not use normalization of params (transform to [0;1] or to [-1;1])
 */
enum GLNormalization
{
    GLN_USE         = GL_TRUE,
    GLN_DO_NOT_USE  = GL_FALSE
};

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
    class GLDataBufferPacker
    {
    private:

        /**
         * Metadata to share per vertex params in vao buffer
         */
        typedef struct VertexData
        {
        public:

            void* data;
            uint32 size;
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
         * @param count Number of BLOCKS (or number of vertexes which are described by this data)
         * @param attributeIndex Index of attribute from shader program (get by GLGPUProgram::getAttributeLocation)
         * @param type Type of the basic data of block (for block vec4f basic data type is GLPT_FLOAT)
         * @param usage Do you want to normalize data (transform to [0;1] or [-1;1])
         */
        void addVertexData(void *data, uint32 size, uint32 count, uint32 attributeIndex,
                           GLParamType type, GLNormalization usage);

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
        void addVertexData(Vector2f* data, uint32 count, uint32 attributeIndex, GLNormalization usage);

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
        void addVertexData(Vector3f* data, uint32 count, uint32 attributeIndex, GLNormalization usage);

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
        void addVertexData(Vector4f* data, uint32 count, uint32 attributeIndex, GLNormalization usage);

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
        uint32 getStride() const;

        /**
         * Get count of described vertexes in the buffer
         *
         * @return Count of vertexes
         */
        uint32 getCount() const;

        /**
         * Total buffer size
         *
         * @note Size of buffer can be evaluated as Stride * count (count of vertexes)
         *
         * @return Size in bytes
         */
        uint32 getTotalBufferSize() const;

        /**
         * Internal buffer with data
         *
         * @warning Only for Vertex Array Object
         *
         * @return Pointer to buffer
         */
        const void* getBuffer() const;

    protected:

        friend class GLVertexArrayObject;

        void* mBuffer;                      // Internal buffer pointer (result packed data buffer)
        int8  mIsInitialized;               // Initialization status
        int8  mIsPacked;                    // Packing status
        uint32 mCount;                      // Count of the described vertexes in the buffer
        uint32 mStride;                     // Number of bytes to describe ONE vertex attributes
        uint32 mTotalSize;                  // Total size of internal buffer in bytes
        ArrayList<VertexData> mMetaData;    // List with data about each vertex attribute

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLVERTEXBUFFER_H