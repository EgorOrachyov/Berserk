//
// Created by Egor Orachyov on 06.07.2018.
//

#include "Buffers/GLElementBufferObject.h"
#include "../../Core/Logging/LogMessages.h"

namespace Berserk
{

    GLElementBufferObject::GLElementBufferObject()
    {
        mHandle = 0;
        mCount = 0;
    }

    GLElementBufferObject::~GLElementBufferObject()
    {
        destroy();
    }

    void GLElementBufferObject::create(uint32 count, uint16* indices, GLPrimitiveMode mode)
    {
        if (!mHandle)
        {
            glGenBuffers(1, &mHandle);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16) * count, indices, GL_STATIC_DRAW);

            mMode = mode;
            mCount = count;
            mType = GLPT_USHORT;
        }
        else
        {
            WARNING("An attempt to create a filled EBO buffer");
        }
    }

    void GLElementBufferObject::create(uint32 count, uint32* indices, GLPrimitiveMode mode)
    {
        if (!mHandle)
        {
            glGenBuffers(1, &mHandle);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * count, indices, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            mMode = mode;
            mCount = count;
            mType = GLPT_UINT;
        }
        else
        {
            WARNING("An attempt to create a filled EBO buffer");
        }
    }

    void GLElementBufferObject::destroy()
    {
        if (mHandle)
        {
            glDeleteBuffers(1, &mHandle);
        }
        mHandle = 0;
        mCount = 0;
    }

    uint32 GLElementBufferObject::getHandle() const
    {
        return mHandle;
    }

    bool GLElementBufferObject::isCreated() const
    {
        return (mHandle != 0);
    }

} // namespace Berserk