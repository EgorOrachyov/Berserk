//
// Created by Egor Orachyov on 09.06.2018.
//

#include "Strings/CHARStaticString.h"
#include "../Strings/StringFlags.h"
#include "../Math/UtilityNumbers.h"

namespace Berserk
{

    CHARStaticString::CHARStaticString()
    {
        mSize = 0;
        mCapacity = sizeof(mBuffer) / sizeof(CHAR);
        mBuffer[0] = '\0';
    }

    CHARStaticString::CHARStaticString(uint32 size, const CHAR* charsBuffer)
    {
        init(size, charsBuffer);
    }

    CHARStaticString::CHARStaticString(const CHAR* charsBuffer)
    {
        init(charsBuffer);
    }

    void CHARStaticString::init(uint32 size, const CHAR *charsBuffer)
    {
        mSize = min(size, mCapacity);
        mCapacity = sizeof(mBuffer) / sizeof(CHAR);
        mBuffer[0] = '\0';

        memcpy(mBuffer, charsBuffer, mCapacity * sizeof(CHAR));
        mBuffer[mSize] = '\0';
    }

    void CHARStaticString::init(const CHAR *charsBuffer)
    {
        mSize = 0;
        mCapacity = sizeof(mBuffer) / sizeof(CHAR);
        mBuffer[0] = '\0';

        while (charsBuffer[mSize] != '\0' && mSize < mCapacity)
        {
            mBuffer[mSize] = charsBuffer[mSize];
            mSize += 1;
        }

        mBuffer[mSize] = '\0';
    }

    void CHARStaticString::init(CHARStaticString &anotherString)
    {
        mSize = anotherString.mSize;
        mCapacity = anotherString.mCapacity;
        memcpy(mBuffer, anotherString.mBuffer, mCapacity * sizeof(CHAR));
    }

    void CHARStaticString::empty()
    {
        mSize = 0;
        mBuffer[0] = '\0';
    }

    void CHARStaticString::copy(CHARStaticString &source)
    {
        memcpy(mBuffer, source.mBuffer, mCapacity * sizeof(CHAR));
        mSize = source.mSize;
        mBuffer[mSize] = '\0';
    }

    void CHARStaticString::copy(CHARStaticString &source, CHARStaticString &mask)
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

    void CHARStaticString::copy(const CHAR *source, uint32 count)
    {
        uint32 toCopy = min(count, mCapacity);
        memcpy(mBuffer, source, toCopy * (sizeof(CHAR)));
        mSize = toCopy;
        mBuffer[mSize] = '\0';
    }

    void CHARStaticString::copy(const CHAR *source)
    {
        mSize = 0;
        while (source[mSize] != '\0' && mSize < mCapacity)
        {
            mBuffer[mSize] = source[mSize];
            mSize += 1;
        }

        mBuffer[mSize] = '\0';
    }

    void CHARStaticString::append(CHARStaticString &source)
    {
        int32 j = 0;
        while (mSize < mCapacity && j < source.mSize)
        { mBuffer[mSize++] = source.mBuffer[j++]; }
        mBuffer[mSize] = '\0';
    }

    void CHARStaticString::append(CHAR symbol)
    {
        if (mSize < mCapacity)
        {
            mBuffer[mSize++] = symbol;
            mBuffer[mSize] = '\0';
        }
    }

    void CHARStaticString::append(const CHAR *source, uint32 count)
    {
        int32 i = 0;
        while (mSize < mCapacity && i < count)
        {
            mBuffer[mSize++] = source[i++];
        }

        mBuffer[mSize] = '\0';
    }

    void CHARStaticString::append(const CHAR *source)
    {
        int32 i = 0;
        while (mSize < mCapacity && source[i] != '\0')
        {
            mBuffer[mSize++] = source[i++];
        }

        mBuffer[mSize] = '\0';
    }

    void CHARStaticString::insert(CHARStaticString &source, uint32 offset)
    {
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
                int32 endToMove = min(mSize + source.mSize, mCapacity);
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

    uint32 CHARStaticString::find(CHARStaticString &subString)
    {
        if (subString.mSize == 0)
        { return StringFindFlags::NOT_FOUND; }

        uint16 i = 0;
        while (i < mSize)
        {
            if (mBuffer[i] == subString.mBuffer[0])
            {
                uint32 j = 1;
                uint32 k = i + 1;
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

    uint32 CHARStaticString::find(CHAR symbol)
    {
        uint32 i = 0;
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

    uint32 CHARStaticString::getSize()
    {
        return mSize;
    }

    uint32 CHARStaticString::getCapacity()
    {
        return mCapacity;
    }

    int32 CHARStaticString::contains(CHAR symbol)
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

    int32 CHARStaticString::getType()
    {
        return StringType::ASCII_STATIC_SIZE;
    }

    const CHAR* CHARStaticString::getCharsBuffer()
    {
        return mBuffer;
    }

} // namespace Berserk