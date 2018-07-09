//
// Created by Egor Orachyov on 09.06.2018.
//

#include "Strings/CStaticString.h"
#include "Strings/StringFlags.h"
#include "Math/UtilityNumbers.h"

namespace Berserk
{

    CStaticString::CStaticString()
    {
        mSize = 0;
        mCapacity = sizeof(mBuffer) / sizeof(CHAR);
        mBuffer[0] = '\0';
    }

    CStaticString::CStaticString(UINT32 size, const CHAR* charsBuffer)
    {
        init(size, charsBuffer);
    }

    CStaticString::CStaticString(const CHAR* charsBuffer)
    {
        init(charsBuffer);
    }

    void CStaticString::init(UINT32 size, const CHAR *charsBuffer)
    {
        mSize = min(size, mCapacity);
        mCapacity = sizeof(mBuffer) / sizeof(CHAR);
        mBuffer[0] = '\0';

        memcpy(mBuffer, charsBuffer, mCapacity * sizeof(CHAR));
        mBuffer[mSize] = '\0';
    }

    void CStaticString::init(const CHAR *charsBuffer)
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

    void CStaticString::init(CStaticString &anotherString)
    {
        mSize = anotherString.mSize;
        mCapacity = anotherString.mCapacity;
        memcpy(mBuffer, anotherString.mBuffer, mCapacity * sizeof(CHAR));
    }

    void CStaticString::empty()
    {
        mSize = 0;
        mBuffer[0] = '\0';
    }

    void CStaticString::copy(CStaticString &source)
    {
        memcpy(mBuffer, source.mBuffer, mCapacity * sizeof(CHAR));
        mSize = source.mSize;
        mBuffer[mSize] = '\0';
    }

    void CStaticString::copy(CStaticString &source, CStaticString &mask)
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

        mBuffer[mSize] = '\0';
    }

    void CStaticString::copy(const CHAR *source, UINT32 count)
    {
        UINT32 toCopy = min(count, mCapacity);
        memcpy(mBuffer, source, toCopy * (sizeof(CHAR)));
        mSize = toCopy;
        mBuffer[mSize] = '\0';
    }

    void CStaticString::copy(const CHAR *source)
    {
        mSize = 0;
        while (source[mSize] != '\0' && mSize < mCapacity)
        {
            mBuffer[mSize] = source[mSize];
            mSize += 1;
        }

        mBuffer[mSize] = '\0';
    }

    void CStaticString::append(CStaticString &source)
    {
        INT32 j = 0;
        while (mSize < mCapacity && j < source.mSize)
        { mBuffer[mSize++] = source.mBuffer[j++]; }
        mBuffer[mSize] = '\0';
    }

    void CStaticString::append(CHAR symbol)
    {
        if (mSize < mCapacity)
        {
            mBuffer[mSize++] = symbol;
            mBuffer[mSize] = '\0';
        }
    }

    void CStaticString::append(const CHAR *source, UINT32 count)
    {
        INT32 i = 0;
        while (mSize < mCapacity && i < count)
        {
            mBuffer[mSize++] = source[i++];
        }

        mBuffer[mSize] = '\0';
    }

    void CStaticString::append(const CHAR *source)
    {
        INT32 i = 0;
        while (mSize < mCapacity && source[i] != '\0')
        {
            mBuffer[mSize++] = source[i++];
        }

        mBuffer[mSize] = '\0';
    }

    void CStaticString::insert(CStaticString &source, UINT32 offset)
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
            mBuffer[mSize] = '\0';
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
                mBuffer[mSize] = '\0';
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
                mBuffer[mSize] = '\0';
            }
        }
    }

    UINT32 CStaticString::find(CStaticString &subString)
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

    UINT32 CStaticString::find(CHAR symbol)
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

    UINT32 CStaticString::getSize()
    {
        return mSize;
    }

    UINT32 CStaticString::getCapacity()
    {
        return mCapacity;
    }

    INT32 CStaticString::contains(CHAR symbol)
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

    INT32 CStaticString::getType()
    {
        return StringType::ASCII_STATIC_SIZE;
    }

    const CHAR* CStaticString::getCharsBuffer()
    {
        return mBuffer;
    }

    CStaticString CStaticString::operator = (const CStaticString& staticString)
    {
        memcpy(mBuffer, staticString.mBuffer, (staticString.mSize + 1) * sizeof(CHAR));
        return *this;
    }

    CStaticString CStaticString::operator + (const CStaticString& staticString) const
    {
        CStaticString s(this->mBuffer);
        s.append(staticString.mBuffer);
        return s;
    }

    CStaticString CStaticString::operator + (CHAR c) const
    {
        CStaticString s(this->mBuffer);
        s.append(c);
        return s;
    }

    const bool CStaticString::operator == (const CStaticString& staticString) const
    {
        if (mSize != staticString.mSize)
        {
            return false;
        }

        UINT32 i = 0;
        while (mBuffer[i] - staticString.mBuffer[i] == 0 && i < mSize)
        {
            i += 1;
        }

        return (i == mSize);
    }


} // namespace Berserk