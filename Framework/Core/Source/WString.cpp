//
// Created by Egor Orachyov on 03.06.2018.
//

#include "Strings/WString.h"
#include "Strings/StringFlags.h"
#include "Strings/WStringBuffer.h"
#include "Math/MathUtility.h"
#include "HashFunctions/CRC32.h"

namespace Berserk
{

    WString::WString()
    {
        UINT16 size = 0;
        WCHAR charsBuffer[] = L"";

        mCapacity = getCapacity(size);
        mSize = (UINT16)Math::min((UINT16)size, mCapacity - 1);

        mBuffer = WStringBuffer::get().getBlock(mCapacity);
        memcpy(mBuffer, charsBuffer, sizeof(WCHAR) * mSize);
        mBuffer[mSize] = L'\0';

        mStringID = hashCRC32((CHAR*)mBuffer, mSize * sizeof(WCHAR));

        //PUSH("Init string %p  size: %u capacity: %u id: %u\n", this, mSize, mCapacity, mStringID);
    }

    WString::WString(UINT32 size, const WCHAR* charsBuffer)
    {
        mCapacity = getCapacity(size);
        mSize = (UINT16)Math::min((UINT16)size, mCapacity - 1);

        mBuffer = WStringBuffer::get().getBlock(mCapacity);
        memcpy(mBuffer, charsBuffer, sizeof(WCHAR) * mSize);
        mBuffer[mSize] = L'\0';

        mStringID = hashCRC32((CHAR*)mBuffer, mSize * sizeof(WCHAR));

        //PUSH("Init string %p  size: %u capacity: %u id: %u\n", this, mSize, mCapacity, mStringID);
    }

    WString::WString(const WCHAR* charsBuffer)
    {
        UINT16 i = 0;
        while (i < SC_MAX_CAPACITY_1024 && charsBuffer[i] != L'\0')
        {
            i += 1;
        }

        mCapacity = getCapacity(i);
        mSize = (UINT16)Math::min(i, mCapacity - 1);

        mBuffer = WStringBuffer::get().getBlock(mCapacity);
        memcpy(mBuffer, charsBuffer, sizeof(WCHAR) * mSize);
        mBuffer[mSize] = L'\0';

        mStringID = hashCRC32((CHAR*)mBuffer, mSize * sizeof(WCHAR));

        //PUSH("Init string %p  size: %u capacity: %u id: %u\n", this, mSize, mCapacity, mStringID);
    }

    WString::~WString()
    {
        //PUSH("Destroy string %p  size: %u capacity: %u id: %u\n", this, mSize, mCapacity, mStringID);
        WStringBuffer::get().returnBlock(mCapacity, mBuffer);
    }

    void WString::empty()
    {
        mSize = 0;
        mStringID = 0;
        mBuffer[0] = L'\0';
    }

    void WString::copy(const WString &source)
    {
        if (mCapacity == source.mCapacity)
        {
            mSize = source.mSize;
            mCapacity = source.mCapacity;
            mStringID = source.mStringID;

            memcpy(mBuffer, source.mBuffer, sizeof(WCHAR) * (mSize + 1));
        }
        else
        {
            WStringBuffer::get().returnBlock(mCapacity, mBuffer);

            mSize = source.mSize;
            mCapacity = source.mCapacity;
            mStringID = source.mStringID;

            mBuffer = WStringBuffer::get().getBlock(mCapacity);
            memcpy(mBuffer, source.mBuffer, sizeof(WCHAR) * (mSize + 1));
        }
    }

    void WString::copy(const WStaticString &source)
    {
        if (mCapacity > source.mSize)
        {
            mSize = (UINT16)source.mSize;
            memcpy(mBuffer, source.mBuffer, sizeof(WCHAR) * (mSize + 1));
            mStringID = hashCRC32((CHAR*)mBuffer, mSize * sizeof(WCHAR));
        }
        else
        {
            WStringBuffer::get().returnBlock(mCapacity, mBuffer);

            mSize = (UINT16)source.mSize;
            mCapacity = getCapacity(source.mSize);

            mBuffer = WStringBuffer::get().getBlock(mCapacity);
            memcpy(mBuffer, source.mBuffer, sizeof(WCHAR) * (mSize + 1));

            mStringID = hashCRC32((CHAR*)mBuffer, mSize * sizeof(WCHAR));
        }
    }

    void WString::append(const WString &source)
    {
        if (mSize + source.mSize >= mCapacity)
        {
            UINT16 newCapacity = getCapacity(mSize + source.mSize);
            WCHAR* newBuffer = WStringBuffer::get().getBlock(newCapacity);

            UINT16 toAppend = (UINT16)Math::min(source.mSize, newCapacity - mSize - 1);

            memcpy(newBuffer, mBuffer, sizeof(WCHAR) * mSize);
            memcpy(newBuffer + mSize, source.mBuffer, sizeof(WCHAR) * (toAppend));

            WStringBuffer::get().returnBlock(mCapacity, mBuffer);

            mSize = mSize + source.mSize;
            mCapacity = newCapacity;
            mBuffer = newBuffer;
            mStringID = hashCRC32((CHAR*)mBuffer, mSize * sizeof(WCHAR));

            mBuffer[mSize] = L'\0';
        }
        else
        {
            memcpy(mBuffer + mSize, source.mBuffer, sizeof(WCHAR) * (source.mSize + 1));

            mSize = mSize + source.mSize;
            mStringID = hashCRC32((CHAR*)mBuffer, mSize * sizeof(WCHAR));
        }
    }

    void WString::append(const WStaticString &source)
    {
        if (mSize + source.mSize >= mCapacity)
        {
            UINT16 newCapacity = getCapacity(mSize + source.mSize);
            WCHAR* newBuffer = WStringBuffer::get().getBlock(newCapacity);

            UINT16 toAppend = (UINT16)Math::min((UINT32)source.mSize, (UINT32)newCapacity - mSize - 1);

            memcpy(newBuffer, mBuffer, sizeof(WCHAR) * mSize);
            memcpy(newBuffer + mSize, source.mBuffer, sizeof(WCHAR) * (toAppend));

            WStringBuffer::get().returnBlock(mCapacity, mBuffer);

            mSize = mSize + (UINT16)source.mSize;
            mCapacity = newCapacity;
            mBuffer = newBuffer;
            mStringID = hashCRC32((CHAR*)mBuffer, mSize * sizeof(WCHAR));

            mBuffer[mSize] = L'\0';
        }
        else
        {
            memcpy(mBuffer + mSize, source.mBuffer, sizeof(WCHAR) * (source.mSize + 1));

            mSize = mSize + (UINT16)source.mSize;
            mStringID = hashCRC32((CHAR*)mBuffer, mSize * sizeof(WCHAR));
        }
    }

    UINT32 WString::find(const WString &subString) const
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

    UINT32 WString::find(const WStaticString &subString) const
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

    UINT32 WString::find(WCHAR symbol) const
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

    UINT32 WString::getSize() const
    {
        return mSize;
    }

    UINT32 WString::getCapacity() const
    {
        return mCapacity;
    }

    INT32 WString::contains(WCHAR symbol) const
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

    INT32 WString::getType() const
    {
        return StringType::ST_CHAR_RESIZEABLE;
    }

    const WCHAR* WString::getChars() const
    {
        return mBuffer;
    }

    WString& WString::operator = (const WString& source)
    {
        copy(source);
        return *this;
    }

    WString& WString::operator += (const WString& source)
    {
        append(source);
        return *this;
    }

    WString& WString::operator += (const WStaticString& source)
    {
        append(source);
        return *this;
    }

    const bool WString::operator == (const WString& source) const
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

    const bool WString::operator == (const WStaticString& source) const
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

    UINT16 WString::getCapacity(UINT32 size)
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