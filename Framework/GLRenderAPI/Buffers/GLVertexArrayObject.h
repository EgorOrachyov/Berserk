//
// Created by Egor Orachyov on 04.07.2018.
//

#ifndef BERSERKENGINE_GLVERTEXARRAYOBJECT_H
#define BERSERKENGINE_GLVERTEXARRAYOBJECT_H

#include "Essential/GLPrimitiveMode.h"

#include "GLDataBufferPacker.h"
#include "GLVertexBufferObject.h"
#include "GLElementBufferObject.h"

namespace Berserk
{

    /**
     * Vertex Array Object (partly wrapper for gl vao). Allows to bind buffers, set
     * attributes' params and push rendering calls to GPU.
     */
    class GLVertexArrayObject
    {
    public:

        GLVertexArrayObject();
        ~GLVertexArrayObject();

        /**
         * Create vao handle
         */
        void init();

        /**
         * Delete vao buffer and vbo buffer if it has been attached via packer
         */
        void destroy();

        /**
         * Call draw function and pass mode and count of vertexes
         */
        void draw();

        /**
         * Call custom draw for internal buffer
         *
         * @param count Number of vertexes to draw
         * @param mode Mode of drawing @see GLPrimitiveMode
         */
        void draw(uint32 count, GLPrimitiveMode mode) const;

        /**
         * Allows to create vbo buffer via packer and bind all the attributes
         *
         * @warning Allows to call only for one packer
         * @warning Do not work if you have already called this function
         * @warning Data should be packed
         *
         * @param packer
         */
        void attachBuffer(GLDataBufferPacker& packer);

        /**
         * Allows to attache VBO buffer and bind all the attributes
         *
         * @param buffer VBO
         */
        void attachBuffer(GLVertexBufferObject& buffer);

        /**
         * Allows to attache EBO buffer and define vertexes' indices
         *
         * @param buffer EBO
         */
        void attachBuffer(GLElementBufferObject& buffer);

        void attachBuffer(GLDataBufferPacker& packer, GLElementBufferObject& buffer);

        /**
         * Detach all binded buffers
         */
        void detach();

        void setDrawingMode(uint32 count, GLPrimitiveMode mode);

        /** @return vao internal handle */
        GLuint getHandle() const;

        /** @return vbo internal handle if it has been created via packer */
        GLuint getBuffer() const;

        /** @return count of vertexes to render in draw() function */
        GLint getCount() const;

        /** @return mode of drawing in draw() @see GLPrimitiveMode */
        GLPrimitiveMode getMode() const;

    protected:

        GLuint mHandle;                 // Internal GL VAO handle
        GLuint mBuffer;                 // Internal GL VBO handle only for packer call
        GLint  mCount;                  // Count of vertexes to render
        GLPrimitiveMode mMode;          // Mode of vertexes to render
        GLParamType mType;
        int8 mUseEBO;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLVERTEXARRAYOBJECT_H