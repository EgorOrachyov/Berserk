//
// Created by Egor Orachyov on 03.06.2018.
//

#include "Strings/CString.h"
#include "Strings/StringFlags.h"
#include "Strings/CStringBuffer.h"
#include "Math/MathUtility.h"
#include "HashFunctions/CRC32.h"

namespace Berserk
{

    CString::CString()
    {
        UINT16 size = 0;
        CHAR charsBuffer[] = "";

        mCapacity = getCapacity(size);
        mSize = size;

        mBuffer = CStringBuffer::get().getBlock(mCapacity);
        mBuffer[mSize] = '\0';

        mStringID = hashCRC32(mBuffer, mSize);

        //PUSH("Init string %s  size: %u capacity: %u id: %u\n", mBuffer, mSize, mCapacity, mStringID);
    }

    CString::CString(UINT32 size, const CHAR* charsBuffer)
    {
        mCapacity = getCapacity(size);
        mSize = (UINT16)Math::min((UINT16)size, mCapacity - 1);

        mBuffer = CStringBuffer::get().getBlock(mCapacity);
        memcpy(mBuffer, charsBuffer, sizeof(CHAR) * mSize);
        mBuffer[mSize] = '\0';

        mStringID = hashCRC32(mBuffer, mSize);

        // todo: consider, that there will be integration with string table

        //PUSH("Init string %s  size: %u capacity: %u id: %u\n", mBuffer, mSize, mCapacity, mStringID);
    }

    CString::CString(const CHAR* charsBuffer)
    {
        UINT16 i = 0;
        while (i < SC_MAX_CAPACITY_1024 && charsBuffer[i] != '\0')
        {
            i += 1;
        }

        mCapacity = getCapacity(i);
        mSize = (UINT16)Math::min(i, mCapacity - 1);

        mBuffer = CStringBuffer::get().getBlock(mCapacity);
        memcpy(mBuffer, charsBuffer, sizeof(CHAR) * mSize);
        mBuffer[mSize] = '\0';

        mStringID = hashCRC32(mBuffer, mSize);

        //PUSH("Init string %s  size: %u capacity: %u id: %u\n", mBuffer, mSize, mCapacity, mStringID);
    }

    CString::~CString()
    {
        //PUSH("Destroy string %s  size: %u capacity: %u id: %u\n", mBuffer, mSize, mCapacity, mStringID);
        CStringBuffer::get().returnBlock(mCapacity, mBuffer);
    }

    void CString::empty()
    {
        mSize = 0;
        mStringID = 0;
        mBuffer[0] = '\0';
    }

    void CString::copy(const CString &source)
    {
        if (mCapacity == source.mCapacity)
        {
            mSize = source.mSize;
            mCapacity = source.mCapacity;
            mStringID = source.mStringID;

            memcpy(mBuffer, source.mBuffer, sizeof(CHAR) * (mSize + 1));
        }
        else
        {
            CStringBuffer::get().returnBlock(mCapacity, mBuffer);

            mSize = source.mSize;
            mCapacity = source.mCapacity;
            mStringID = source.mStringID;

            mBuffer = CStringBuffer::get().getBlock(mCapacity);
            memcpy(mBuffer, source.mBuffer, sizeof(CHAR) * (mSize + 1));
        }
    }

    void CString::copy(const CStaticString &source)
    {
        CStringBuffer::get().returnBlock(mCapacity, mBuffer);

        mSize = (UINT16)source.mSize;
        mCapacity = getCapacity(source.mSize);
        mBuffer = CStringBuffer::get().getBlock(mCapacity);

        memcpy(mBuffer, source.mBuffer, sizeof(CHAR) * (mSize + 1));

        mStringID = hashCRC32(mBuffer, mSize);
    }

    void CString::append(const CString &source)
    {
        if (mSize + source.mSize >= mCapacity)
        {
            UINT16 newCapacity = getCapacity(mSize + source.mSize);
            CHAR* newBuffer = CStringBuffer::get().getBlock(newCapacity);

            UINT16 toAppend = (UINT16)Math::min(source.mSize, newCapacity - mSize - 1);

            memcpy(newBuffer, mBuffer, sizeof(CHAR) * mSize);
            memcpy(newBuffer + mSize, source.mBuffer, sizeof(CHAR) * (toAppend));

            CStringBuffer::get().returnBlock(mCapacity, mBuffer);

            mSize = mSize + source.mSize;
            mCapacity = newCapacity;
            mBuffer = newBuffer;
            mStringID = hashCRC32(mBuffer, mSize);

            mBuffer[mSize] = '\0';
        }
        else
        {
            memcpy(mBuffer + mSize, source.mBuffer, sizeof(CHAR) * (source.mSize + 1));

            mSize = mSize + source.mSize;
            mStringID = hashCRC32(mBuffer, mSize);
        }
    }

    void CString::append(const CStaticString &source)
    {
        if (mSize + source.mSize >= mCapacity)
        {
            UINT16 newCapacity = getCapacity(mSize + source.mSize);
            CHAR* newBuffer = CStringBuffer::get().getBlock(newCapacity);

            UINT16 toAppend = (UINT16)Math::min((UINT32)source.mSize, (UINT32)newCapacity - mSize - 1);

            memcpy(newBuffer, mBuffer, sizeof(CHAR) * mSize);
            memcpy(newBuffer + mSize, source.mBuffer, sizeof(CHAR) * (toAppend));

            CStringBuffer::get().returnBlock(mCapacity, mBuffer);

            mSize = mSize + (UINT16)source.mSize;
            mCapacity = newCapacity;
            mBuffer = newBuffer;
            mStringID = hashCRC32(mBuffer, mSize);

            mBuffer[mSize] = '\0';
        }
        else
        {
            memcpy(mBuffer + mSize, source.mBuffer, sizeof(CHAR) * (source.mSize + 1));

            mSize = mSize + (UINT16)source.mSize;
            mStringID = hashCRC32(mBuffer, mSize);
        }
    }

    UINT32 CString::find(const CString &subString) const
    {
        if (subString.mSize > mSize || subString.mSize == 0)
        {
            return StringFindFlags::NOT_FOUND;
        }

        UINT32 i = 0;
        while (i < mSize)
        {
            if (mBuffer[i] == subString.mBuffer[0])
            {
                UINT32 k = 1;
                UINT32 j = i + 1;

                UINT32 found = StringFindFlags::FOUND;

                while (j < mSize && k < subString.mSize)
                {
                    if (mBuffer[j] != mBuffer[k])
                    {
                        found = StringFindFlags::NOT_FOUND;
                        break;
                    }

                    j += 1;
                    k += 1;
                }

                if (found == StringFindFlags::FOUND && k == subString.mSize)
                {
                    return i;
                }
            }

            i += 1;
        }

        return StringFindFlags::NOT_FOUND;
    }

    UINT32 CString::find(const CStaticString &subString) const
    {
        if (subString.mSize > mSize || subString.mSize == 0)
        {
            return StringFindFlags::NOT_FOUND;
        }

        UINT32 i = 0;
        while (i < mSize)
        {
            if (mBuffer[i] == subString.mBuffer[0])
            {
                UINT32 k = 1;
                UINT32 j = i + 1;

                UINT32 found = StringFindFlags::FOUND;

                while (j < mSize && k < subString.mSize)
                {
                    if (mBuffer[j] != mBuffer[k])
                    {
                        found = StringFindFlags::NOT_FOUND;
                        break;
                    }

                    j += 1;
                    k += 1;
                }

                if (found == StringFindFlags::FOUND && k == subString.mSize)
                {
                    return i;
                }
            }

            i += 1;
        }

        return StringFindFlags::NOT_FOUND;
    }

    UINT32 CString::find(CHAR symbol) const
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

    UINT32 CString::getSize() const
    {
        return mSize;
    }

    UINT32 CString::getCapacity() const
    {
        return mCapacity;
    }

    INT32 CString::contains(CHAR symbol) const
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

    INT32 CString::getType() const
    {
        return StringType::ST_CHAR_RESIZEABLE;
    }

    const CHAR* CString::getChars() const
    {
        return mBuffer;
    }

    CString& CString::operator = (const CString& source)
    {
        copy(source);
        return *this;
    }

    CString& CString::operator = (const CStaticString& source)
    {
        copy(source);
        return *this;
    }

    CString& CString::operator += (const CString& source)
    {
        append(source);
        return *this;
    }

    CString& CString::operator += (const CStaticString& source)
    {
        append(source);
        return *this;
    }

    const bool CString::operator == (const CString& source) const
    {
        if (mSize != source.mSize)
        {
            return false;
        }

        UINT32 i = 0;
        while (mBuffer[i] - source.mBuffer[i] == 0 && i < mSize)
        {
            i += 1;
        }

        return (i == mSize);
    }

    const bool CString::operator==(const CStaticString &source) const
    {
        if (mSize != source.mSize)
        {
            return false;
        }

        UINT32 i = 0;
        while (mBuffer[i] - source.mBuffer[i] == 0 && i < mSize)
        {
            i += 1;
        }

        return (i == mSize);
    }

    UINT16 CString::getCapacity(UINT32 size)
    {
        // todo: add more beautiful solution

        if (size < SC_MAX_CAPACITY_32)
        {
            return SC_MAX_CAPACITY_32;
        }
        else if (size < SC_MAX_CAPACITY_64)
        {
            return SC_MAX_CAPACITY_64;
        }
        else if (size < SC_MAX_CAPACITY_128)
        {
            return SC_MAX_CAPACITY_128;
        }
        else if (size < SC_MAX_CAPACITY_256)
        {
            return SC_MAX_CAPACITY_256;
        }
        else if (size < SC_MAX_CAPACITY_512)
        {
            return SC_MAX_CAPACITY_512;
        }
        else
        {
            return SC_MAX_CAPACITY_1024;
        }
    }

} // namespace Berserk