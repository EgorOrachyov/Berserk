//
// Created by Egor Orachyov on 04.07.2018.
//

#include "GLDataBufferPacker.h"

#include "../../Core/Logging/LogMessages.h"

namespace Berserk
{

    GLDataBufferPacker::GLDataBufferPacker()
    {
        mBuffer = NULL;
        mIsInitialized = false;
        mIsPacked = false;
        mTotalSize = 0;
        mStride = 0;
        mCount = 0;
    }

    GLDataBufferPacker::~GLDataBufferPacker()
    {
        destroy();
    }

    void GLDataBufferPacker::init()
    {
        mMetaData.init();
        mIsInitialized = true;
    }

    void GLDataBufferPacker::destroy()
    {
        reset();
    }

    void GLDataBufferPacker::pack()
    {
        if (!mBuffer && mMetaData.getSize() > 0)
        {
            mBuffer = mem_alloc(mTotalSize);
            uint32 offset = 0;

            for(uint32 i = 0; i < mMetaData.getSize(); i++)
            {
                VertexData* current = mMetaData.getPointer(i);

                for(uint32 j = 0; j < mCount; j++)
                {
                    memcpy((int8*)mBuffer + mStride * j + offset, (int8*)current->data + current->size * j, current->size);
                }

                current->offset = (void*)offset;
                offset += current->size;
            }

            mIsPacked = true;
        }
        else
        {
            WARNING("An attempt to pack NULL data");
        }
    }

    void GLDataBufferPacker::reset()
    {
        if (mBuffer)
        {
            mem_free(mBuffer);
        }
        mMetaData.empty();
        mBuffer = NULL;
        mTotalSize = 0;
        mStride = 0;
        mCount = 0;
    }

    void GLDataBufferPacker::addVertexData(void *data, uint32 size, uint32 count, uint32 attributeIndex,
                                           GLParamType type, GLNormalization usage)
    {
        if (mIsInitialized)
        {
            if (mMetaData.getSize() == 0)
            {
                VertexData vd = {data, size, attributeIndex, type, (GLboolean)usage, NULL};
                mMetaData.add(vd);
                mTotalSize += size * count;
                mStride += size;
                mCount = count;
            }
            else if (mCount != count)
            {
                ERROR("Count of element in the buffer do not incompatible with previously got value");
            }
            else
            {
                VertexData vd = {data, size, attributeIndex, type, (GLboolean)usage, NULL};
                mMetaData.add(vd);
                mTotalSize += size * count;
                mStride += size;
            }
        }
    }

    void GLDataBufferPacker::addVertexData(Vector2f* data, uint32 count, uint32 attributeIndex, GLNormalization usage)
    {
        if (mMetaData.getSize() == 0)
        {
            uint32 size = sizeof(float32) * 2;

            VertexData vd = {(void*)data, size, attributeIndex, GLPT_FLOAT, (GLboolean)usage, NULL};
            mMetaData.add(vd);
            mTotalSize += size * count;
            mStride += size;
            mCount = count;
        }
        else if (mCount != count)
        {
            ERROR("Count of element in the buffer do not incompatible with previously got value");
        }
        else
        {
            uint32 size = sizeof(float32) * 2;

            VertexData vd = {(void*)data, size, attributeIndex, GLPT_FLOAT, (GLboolean)usage, NULL};
            mMetaData.add(vd);
            mTotalSize += size * count;
            mStride += size;
        }
    }

    void GLDataBufferPacker::addVertexData(Vector3f* data, uint32 count, uint32 attributeIndex, GLNormalization usage)
    {
        if (mMetaData.getSize() == 0)
        {
            uint32 size = sizeof(float32) * 3;

            VertexData vd = {(void*)data, size, attributeIndex, GLPT_FLOAT, (GLboolean)usage, NULL};
            mMetaData.add(vd);
            mTotalSize += size * count;
            mStride += size;
            mCount = count;
        }
        else if (mCount != count)
        {
            ERROR("Count of element in the buffer do not incompatible with previously got value");
        }
        else
        {
            uint32 size = sizeof(float32) * 3;

            VertexData vd = {(void*)data, size, attributeIndex, GLPT_FLOAT, (GLboolean)usage, NULL};
            mMetaData.add(vd);
            mTotalSize += size * count;
            mStride += size;
        }
    }

    void GLDataBufferPacker::addVertexData(Vector4f* data, uint32 count, uint32 attributeIndex, GLNormalization usage)
    {
        if (mMetaData.getSize() == 0)
        {
            uint32 size = sizeof(float32) * 4;

            VertexData vd = {(void*)data, size, attributeIndex, GLPT_FLOAT, (GLboolean)usage, NULL};
            mMetaData.add(vd);
            mTotalSize += size * count;
            mStride += size;
            mCount = count;
        }
        else if (mCount != count)
        {
            ERROR("Count of element in the buffer do not incompatible with previously got value");
        }
        else
        {
            uint32 size = sizeof(float32) * 4;

            VertexData vd = {(void*)data, size, attributeIndex, GLPT_FLOAT, (GLboolean)usage, NULL};
            mMetaData.add(vd);
            mTotalSize += size * count;
            mStride += size;
        }
    }

    bool GLDataBufferPacker::isInitialized() const
    {
        return mIsInitialized;
    }

    bool GLDataBufferPacker::isPacked() const
    {
        return mIsPacked;
    }

    uint32 GLDataBufferPacker::getStride() const
    {
        return mStride;
    }

    uint32 GLDataBufferPacker::getCount() const
    {
        return mCount;
    }

    uint32 GLDataBufferPacker::getTotalBufferSize() const
    {
        return mTotalSize;
    }

    const void* GLDataBufferPacker::getBuffer() const
    {
        return mBuffer;
    }

} // namespace Berserk