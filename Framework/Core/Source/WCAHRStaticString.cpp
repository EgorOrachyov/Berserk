//
// Created by Egor Orachyov on 26.06.2018.
//

#include "Strings/WCAHRStaticString.h"
#include "../Strings/StringFlags.h"
#include "../Math/UtilityNumbers.h"

namespace Berserk
{

    WCAHRStaticString::WCAHRStaticString()
    {
        mSize = 0;
        mCapacity = sizeof(mBuffer) / sizeof(WCHAR);
        mBuffer[0] = L'\0';
    }

    WCAHRStaticString::WCAHRStaticString(UINT32 size, const WCHAR* charsBuffer)
    {
        init(size, charsBuffer);
    }

    WCAHRStaticString::WCAHRStaticString(const WCHAR* charsBuffer)
    {
        init(charsBuffer);
    }

    void WCAHRStaticString::init(UINT32 size, const WCHAR *charsBuffer)
    {
        mSize = min(size, mCapacity);
        mCapacity = sizeof(mBuffer) / sizeof(WCHAR);
        mBuffer[0] = L'\0';

        memcpy(mBuffer, charsBuffer, mCapacity * sizeof(WCHAR));
        mBuffer[mSize] = L'\0';
    }

    void WCAHRStaticString::init(const WCHAR *charsBuffer)
    {
        mSize = 0;
        mCapacity = sizeof(mBuffer);
        mBuffer[0] = L'\0';

        while (charsBuffer[mSize] != L'\0' && mSize < mCapacity)
        {
            mBuffer[mSize] = charsBuffer[mSize];
            mSize += 1;
        }

        mBuffer[mSize] = L'\0';
    }

    void WCAHRStaticString::init(WCAHRStaticString &anotherString)
    {
        mSize = anotherString.mSize;
        mCapacity = anotherString.mCapacity;
        memcpy(mBuffer, anotherString.mBuffer, mCapacity * sizeof(WCHAR));
    }

    void WCAHRStaticString::empty()
    {
        mSize = 0;
        mBuffer[0] = L'\0';
    }

    void WCAHRStaticString::copy(WCAHRStaticString &source)
    {
        memcpy(mBuffer, source.mBuffer, mCapacity * sizeof(WCHAR));
        mSize = source.mSize;
        mBuffer[mSize] = L'\0';
    }

    void WCAHRStaticString::copy(WCAHRStaticString &source, WCAHRStaticString &mask)
    {
        mSize = 0;
        INT32 k = 0;

        while (k < mask.mSize && mSize < mCapacity)
        {
            if (mask.mBuffer[k] == '%' && mask.mBuffer[k + 1] == 's')
            {
                INT32 i = 0;
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

        mBuffer[mSize] = L'\0';
    }

    void WCAHRStaticString::copy(const WCHAR *source, UINT32 count)
    {
        UINT32 toCopy = min(count, mCapacity);
        memcpy(mBuffer, source, toCopy * (sizeof(WCHAR)));
        mSize = toCopy;
        mBuffer[mSize] = L'\0';
    }

    void WCAHRStaticString::copy(const WCHAR *source)
    {
        mSize = 0;
        while (source[mSize] != L'\0' && mSize < mCapacity)
        {
            mBuffer[mSize] = source[mSize];
            mSize += 1;
        }

        mBuffer[mSize] = L'\0';
    }

    void WCAHRStaticString::append(WCAHRStaticString &source)
    {
        INT32 j = 0;
        while (mSize < mCapacity && j < source.mSize)
        { mBuffer[mSize++] = source.mBuffer[j++]; }
        mBuffer[mSize] = L'\0';
    }

    void WCAHRStaticString::append(WCHAR symbol)
    {
        if (mSize < mCapacity)
        {
            mBuffer[mSize++] = symbol;
            mBuffer[mSize] = L'\0';
        }
    }

    void WCAHRStaticString::append(const WCHAR *source, UINT32 count)
    {
        INT32 i = 0;
        while (mSize < mCapacity && i < count)
        {
            mBuffer[mSize++] = source[i++];
        }

        mBuffer[mSize] = L'\0';
    }

    void WCAHRStaticString::append(const WCHAR *source)
    {
        INT32 i = 0;
        while (mSize < mCapacity && source[i] != L'\0')
        {
            mBuffer[mSize++] = source[i++];
        }

        mBuffer[mSize] = L'\0';
    }

    void WCAHRStaticString::insert(WCAHRStaticString &source, UINT32 offset)
    {
        if (offset > mSize)
        {
            return;
        }
        else if (mSize == offset)
        {
            INT32 i = 0;
            while (i < source.mSize && mSize < mCapacity)
            {
                mBuffer[mSize++] = source.mBuffer[i++];
            }
            mBuffer[mSize] = L'\0';
        }
        else
        {
            if (offset + source.mSize >= mCapacity)
            {
                INT32 i = offset;
                INT32 j = 0;
                while (i < mCapacity && j < source.mSize)
                {
                    mBuffer[i++] = source.mBuffer[j++];
                }
                mSize = mCapacity;
                mBuffer[mSize] = L'\0';
            }
            else
            {
                INT32 endToMove = min(mSize + source.mSize, mCapacity);
                INT32 beginToMove = offset + source.mSize;

                INT32 i = offset;
                INT32 j = beginToMove;
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
                mBuffer[mSize] = L'\0';
            }
        }
    }

    UINT32 WCAHRStaticString::find(WCAHRStaticString &subString)
    {
        if (subString.mSize == 0)
        { return StringFindFlags::NOT_FOUND; }

        UINT16 i = 0;
        while (i < mSize)
        {
            if (mBuffer[i] == subString.mBuffer[0])
            {
                UINT32 j = 1;
                UINT32 k = i + 1;
                INT16 found = StringFindFlags::FOUND;
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

    UINT32 WCAHRStaticString::find(WCHAR symbol)
    {
        UINT32 i = 0;
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

    UINT32 WCAHRStaticString::getSize()
    {
        return mSize;
    }

    UINT32 WCAHRStaticString::getCapacity()
    {
        return mCapacity;
    }

    INT32 WCAHRStaticString::contains(WCHAR symbol)
    {
        INT32 i = 0;
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

    INT32 WCAHRStaticString::getType()
    {
        return StringType::ASCII_STATIC_SIZE;
    }

    const WCHAR* WCAHRStaticString::getCharsBuffer()
    {
        return mBuffer;
    }

} // namespace Berserk