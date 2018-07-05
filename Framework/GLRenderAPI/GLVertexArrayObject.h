//
// Created by Egor Orachyov on 04.07.2018.
//

#ifndef BERSERKENGINE_GLVERTEXARRAYOBJECT_H
#define BERSERKENGINE_GLVERTEXARRAYOBJECT_H

#include "GLInclude.h"
#include "GLDataBufferPacker.h"

enum GLPrimitiveMode
{
    GLPM_TRIANGLES = GL_TRIANGLES
};


namespace Berserk
{

    class GLVertexArrayObject
    {
    public:

        GLVertexArrayObject();
        ~GLVertexArrayObject();

        void init();
        void destroy();
        void draw();
        void draw(uint32 count, GLPrimitiveMode mode);

        void attachBuffer(GLDataBufferPacker& packer);

        const GLuint getHandle() const;

    protected:

        GLuint mHandle;
        uint32 mCount;
        GLPrimitiveMode mMode;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLVERTEXARRAYOBJECT_H