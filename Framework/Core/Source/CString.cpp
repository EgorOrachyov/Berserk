//
// Created by Egor Orachyov on 03.06.2018.
//

#include "Strings/CString.h"
#include "Strings/StringFlags.h"
#include "Strings/CStringBuffer.h"
#include "Math/UtilityNumbers.h"
#include "HashFunctions/CRC32.h"

namespace Berserk
{

    CString::CString(UINT32 size, const CHAR* charsBuffer)
    {
        mCapacity = getCapacity(size);
        mSize = (UINT16)min((UINT16)size, mCapacity - 1);

        mBuffer = CStringBuffer::get().getBlock(mCapacity);
        memcpy(mBuffer, charsBuffer, sizeof(CHAR) * mSize);
        mBuffer[mSize] = '\0';

        mStringID = hashCRC32(mBuffer, mSize);

        // todo: consider, that there will be integration with string table

        PUSH("Init string %s  size: %u capacity: %u id: %u\n", mBuffer, mSize, mCapacity, mStringID);
    }

    CString::CString(const CHAR* charsBuffer)
    {
        UINT16 i = 0;
        while (i < SC_MAX_CAPACITY_1024 && charsBuffer[i] != '\0')
        {
            i += 1;
        }

        mCapacity = getCapacity(i);
        mSize = (UINT16)min(i, mCapacity - 1);

        mBuffer = CStringBuffer::get().getBlock(mCapacity);
        memcpy(mBuffer, charsBuffer, sizeof(CHAR) * mSize);
        mBuffer[mSize] = '\0';

        mStringID = hashCRC32(mBuffer, mSize);

        PUSH("Init string %s  size: %u capacity: %u id: %u\n", mBuffer, mSize, mCapacity, mStringID);

    }

    CString::~CString()
    {
        PUSH("Destroy string %s  size: %u capacity: %u id: %u\n", mBuffer, mSize, mCapacity, mStringID);
        CStringBuffer::get().returnBlock(mCapacity, mBuffer);
    }

    void CString::empty()
    {
        mSize = 0;
        mStringID = 0;
        mBuffer[0] = '\0';
    }

    void CString::copy(CString &source)
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

    void CString::copy(CStaticString &source)
    {
        if (mCapacity > source.mSize)
        {
            mSize = (UINT16)source.mSize;
            memcpy(mBuffer, source.mBuffer, sizeof(CHAR) * (mSize + 1));
            mStringID = hashCRC32(mBuffer, mSize);
        }
        else
        {
            CStringBuffer::get().returnBlock(mCapacity, mBuffer);

            mSize = (UINT16)source.mSize;
            mCapacity = getCapacity(source.mSize);

            mBuffer = CStringBuffer::get().getBlock(mCapacity);
            memcpy(mBuffer, source.mBuffer, sizeof(CHAR) * (mSize + 1));

            mStringID = hashCRC32(mBuffer, mSize);
        }
    }

    void CString::append(CString &source)
    {

    }

    void CString::append(CStaticString &source)
    {

    }

    UINT32 CString::find(CString &subString)
    {

    }

    UINT32 CString::find(CStaticString &subString)
    {

    }

    UINT32 CString::find(CHAR symbol)
    {

    }

    UINT32 CString::getSize()
    {

    }

    UINT32 CString::getCapacity()
    {

    }

    INT32 CString::contains(CHAR symbol)
    {

    }

    INT32 CString::getType()
    {

    }

    const CHAR* CString::getCharsBuffer()
    {

    }

    CString CString::operator = (const CString& source)
    {

    }

    CString CString::operator + (const CString& source) const
    {

    }

    CString CString::operator + (const CStaticString& source) const
    {

    }

    CString CString::operator + (CHAR c) const
    {

    }

    const bool CString::operator == (const CString& source) const
    {

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