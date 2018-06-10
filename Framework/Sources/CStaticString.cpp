//
// Created by Egor Orachyov on 09.06.2018.
//

#include "../Core/Strings/CStaticString.h"
#include "../Core/Strings/StringFlags.h"

namespace Berserk
{

    CStaticString::CStaticString()
    {
        mSize = 0;
        mCapacity = 0;
        mHash = 0;
        mBuffer = 0;
    }

    CStaticString::~CStaticString()
    {
        Delete();
    }

    void CStaticString::Init(uint16 size, uint16 capacity, uint32 hash, int8* charsBuffer)
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
        if (mCapacity > 0)
        {
            uint16 toCopy = (uint16)(mCapacity > source.mSize? source.mSize + 1 : mCapacity + 1);
            memcpy(mBuffer, source.mBuffer, toCopy * sizeof(int8));
            mSize = toCopy;
        }
    }

    void CStaticString::Copy(CStaticString source, CStaticString mask)
    {
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

    void CStaticString::Append(CStaticString source)
    {
        int32 j = 0;
        while (mSize < mCapacity && j < source.mSize)
        { mBuffer[mSize++] = source.mBuffer[j++]; }
        mBuffer[mSize] = '\0';
    }

    void CStaticString::Append(int8 symbol)
    {
        if (mSize < mCapacity)
        {
            mBuffer[mSize++] = symbol;
            mBuffer[mSize] = '\0';
        }
    }

    void CStaticString::Insert(CStaticString source, uint16 offset)
    {

    }

    uint16 CStaticString::Find(CStaticString subString)
    {
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

                if (found == StringFindFlags::FOUND && k == mSize)
                {
                    return i;
                }
            }

            i += 1;
        }

        return StringFindFlags::NOT_FOUND;
    }

    uint16 CStaticString::Find(int8 symbol)
    {
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

    int32 CStaticString::Contains(int8 symbol)
    {
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

    const int8* CStaticString::GetCharsBuffer()
    {
        return mBuffer;
    }

} // namespace Berserk