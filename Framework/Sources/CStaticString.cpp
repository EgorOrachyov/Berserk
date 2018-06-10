//
// Created by Egor Orachyov on 09.06.2018.
//

#include "../Core/Strings/CStaticString.h"
#include "../Core/Strings/StringFlags.h"
#include "../Core/Essential/Assert.h"

namespace Berserk
{

    CStaticString::CStaticString()
    {
        mSize = 0;
        mCapacity = 0;
        mHash = 0;
        mBuffer = 0;

        printf("C string constructor \n");
    }

    CStaticString::~CStaticString()
    {
        Delete();
        printf("C string destructor %s\n", mBuffer);
    }

    void CStaticString::Init(uint16 size, uint16 capacity, uint32 hash, CHAR *charsBuffer)
    {
        mSize = size;
        mCapacity = capacity;
        mHash = hash;
        mBuffer = charsBuffer;
    }

    void CStaticString::Delete()
    {

    }

    void CStaticString::Empty()
    {
        if (mCapacity > 0)
        {
            mSize = 0;
            mHash = 0;
            mBuffer[0] = '\0';
        }
    }

    void CStaticString::Copy(CStaticString source)
    {
        ASSERT(mCapacity > 0, "String should be initialized");

        if (mCapacity > 0)
        {
            uint16 toCopy = (mCapacity > source.mSize? source.mSize : mCapacity);
            memcpy(mBuffer, source.mBuffer, toCopy * sizeof(CHAR));
            mSize = toCopy;
            mBuffer[mSize] = '\0';
        }
    }

    void CStaticString::Copy(CStaticString source, CStaticString mask)
    {
        ASSERT(mCapacity > 0, "String should be initialized");

        if (mCapacity > 0)
        {
            mSize = 0;
            int32 k = 0;

            while (k < mask.mSize && mSize < mCapacity)
            {
                if (mask.mBuffer[k] == '%' && mask.mBuffer[k + 1] == 's')
                {
                    int32 i = 0;
                    while (mSize < mCapacity && i < source.mSize)
                    {
                        mBuffer[mSize] = source.mBuffer[i];

                        i += 1;
                        mSize += 1;
                    }

                    k += 2;
                }
                else
                {
                    mBuffer[mSize] = mask.mBuffer[k];

                    mSize += 1;
                    k += 1;
                }
            }

            mBuffer[mSize] = '\0';
        }
    }

    void CStaticString::Copy(CHAR* source, uint16 count)
    {
        ASSERT(mCapacity > 0, "String should be initialized");

        if (mCapacity > 0)
        {
            uint16 toCopy = (mCapacity > count? count : mCapacity);
            memcpy(mBuffer, source, toCopy * (sizeof(CHAR)));
            mSize = toCopy;
            mBuffer[mSize] = '\0';
        }
    }

    void CStaticString::Append(CStaticString source)
    {
        ASSERT(mCapacity > 0, "String should be initialized");

        int32 j = 0;
        while (mSize < mCapacity && j < source.mSize)
        { mBuffer[mSize++] = source.mBuffer[j++]; }
        mBuffer[mSize] = '\0';
    }

    void CStaticString::Append(CHAR symbol)
    {
        ASSERT(mCapacity > 0, "String should be initialized");

        if (mSize < mCapacity)
        {
            mBuffer[mSize++] = symbol;
            mBuffer[mSize] = '\0';
        }
    }

    void CStaticString::Append(CHAR *source, uint16 count)
    {
        ASSERT(mCapacity > 0, "String should be initialized");

        int32 i = 0;
        while (mSize < mCapacity && i < count)
        {
            mBuffer[mSize++] = source[i++];
        }

        mBuffer[mSize] = '\0';
    }

    void CStaticString::Insert(CStaticString source, uint16 offset)
    {
        ASSERT(mCapacity > 0, "String should be initialized");

        if (offset > mSize)
        {
            return;
        }
        else if (mSize == offset)
        {
            int32 i = 0;
            while (i < source.mSize && mSize < mCapacity)
            {
                mBuffer[mSize++] = source.mBuffer[i++];
            }
            mBuffer[mSize] = '\0';
        }
        else
        {
            if (offset + source.mSize >= mCapacity)
            {
                int32 i = offset;
                int32 j = 0;
                while (i < mCapacity && j < source.mSize)
                {
                    mBuffer[i++] = source.mBuffer[j++];
                }
                mSize = mCapacity;
                mBuffer[mSize] = '\0';
            }
            else
            {
                int32 endToMove = (mCapacity > (mSize + source.mSize) ? mSize + source.mSize : mCapacity);
                int32 beginToMove = offset + source.mSize;

                int32 i = offset;
                int32 j = beginToMove;
                while (j < endToMove)
                {
                    mBuffer[j++] = mBuffer[i++];
                }

                i = offset;
                j = 0;
                while (j < source.mSize)
                {
                    mBuffer[i++] = source.mBuffer[j++];
                }

                mSize += source.mSize;
                mBuffer[mSize] = '\0';
            }
        }
    }

    uint16 CStaticString::Find(CStaticString subString)
    {
        ASSERT(mCapacity > 0, "String should be initialized");

        if (subString.mSize == 0)
        { return StringFindFlags::NOT_FOUND; }

        uint16 i = 0;
        while (i < mSize)
        {
            if (mBuffer[i] == subString.mBuffer[0])
            {
                uint16 j = 1;
                uint16 k = i + 1;
                int16 found = StringFindFlags::FOUND;
                while (j < subString.mSize && k < mSize)
                {
                    if (mBuffer[k] != subString.mBuffer[j])
                    {
                        found = StringFindFlags::NOT_FOUND;
                        break;
                    }

                    j += 1;
                    k += 1;
                }

                if (found == StringFindFlags::FOUND && j == subString.mSize)
                {
                    return i;
                }
            }

            i += 1;
        }

        return StringFindFlags::NOT_FOUND;
    }

    uint16 CStaticString::Find(CHAR symbol)
    {
        ASSERT(mCapacity > 0, "String should be initialized");

        uint16 i = 0;
        while (i < mSize)
        {
            if (mBuffer[i] == symbol)
            {
                return i;
            }

            i += 1;
        }

        return StringFindFlags::NOT_FOUND;
    }

    uint16 CStaticString::GetSize()
    {
        return mSize;
    }

    uint16 CStaticString::GetCapacity()
    {
        return mCapacity;
    }

    uint32 CStaticString::GetHash()
    {
        return mHash;
    }

    int32 CStaticString::Contains(CHAR symbol)
    {
        ASSERT(mCapacity > 0, "String should be initialized");

        int32 i = 0;
        while (i < mSize)
        {
            if (mBuffer[i] == symbol)
            {
                return StringFindFlags::FOUND;
            }

            i += 1;
        }

        return StringFindFlags::NOT_FOUND;
    }

    int32 CStaticString::GetType()
    {
        return StringType::ASCII_STATIC_SIZE;
    }

    const CHAR* CStaticString::GetCharsBuffer()
    {
        ASSERT(mCapacity > 0, "String should be initialized");

        return mBuffer;
    }

} // namespace Berserk