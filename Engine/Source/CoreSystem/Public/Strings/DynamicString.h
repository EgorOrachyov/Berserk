//
// Created by Egor Orachyov on 29.01.2019.
//

#ifndef BERSERK_DYNAMICSTRING_H
#define BERSERK_DYNAMICSTRING_H

#include "Logging/LogMacros.h"
#include "Strings/StringPool.h"
#include "Strings/StringUtility.h"

namespace Berserk
{

    template <typename T, T end>
    class DynamicString
    {
    public:

        DynamicString();

        explicit DynamicString(const T* source);

        DynamicString(const DynamicString& source);

        ~DynamicString();

    public:

        void empty();

        void instant();

        void operator += (const T *source);

        void operator += (const DynamicString& source);

        DynamicString& operator = (const T *source);

        DynamicString& operator = (const DynamicString& source);

        uint32 length() const;

        uint32 capacity() const;

        uint32 referenceCount() const;

        T* get() const;

    private:

        typedef Strings<T, end> Utils;

        T* mBuffer;                     // String data
        StringPool::PoolNode* mNode;    // String info

    };

    template <typename T, T end>
    DynamicString<T,end>::DynamicString()
    {
        mNode = StringPool::getSingleton().create();
        mBuffer = (T*) StringPool::getSingleton().getBufferPtr(mNode);
    }

    template <typename T, T end>
    DynamicString<T,end>::DynamicString(const T *source)
    {
        uint32 length = Utils::strlen(source);

        mNode = StringPool::getSingleton().allocate(sizeof(T) * length);
        mBuffer = (T*) StringPool::getSingleton().getBufferPtr(mNode);
        mNode->mLength = (uint16) length;

        Utils::strcpy(mBuffer, source);
    }

    template <typename T, T end>
    DynamicString<T,end>::DynamicString(const DynamicString &source)
    {
        mNode = source.mNode;
        mBuffer = source.mBuffer;
        mNode->mReferenceCount += 1;
    }

    template <typename T, T end>
    DynamicString<T,end>::~DynamicString()
    {
        if (mNode)
        {
            PUSH("DynamicString: delete [string: '%s'][ref: %u]", (char*)mBuffer, mNode->mReferenceCount);

            StringPool::getSingleton().free(mNode);

            mNode = nullptr;
            mBuffer = nullptr;
        }
    }

    template <typename T, T end>
    void DynamicString<T,end>::empty()
    {
        StringPool::getSingleton().free(mNode);
        mNode = StringPool::getSingleton().create();
        mBuffer = StringPool::getSingleton().getBufferPtr(mNode);
    }

    template <typename T, T end>
    void DynamicString<T,end>::instant()
    {
        if (mNode->mReferenceCount == 1)
        {
            return;
        }

        StringPool::PoolNode* oldNode = mNode;
        T* oldBuffer = mBuffer;

        mNode = StringPool::getSingleton().allocate(sizeof(T) * oldNode->mLength);
        mBuffer = StringPool::getSingleton().getBufferPtr(mNode);
        mNode->mLength = oldNode->mLength;

        Utils::strcpy(mBuffer, oldBuffer);

        StringPool::getSingleton().free(oldNode);
    }

    template <typename T, T end>
    void DynamicString<T,end>::operator+=(const T *source)
    {
        uint32 length = mNode->mLength + Utils::strlen(source);

        if (sizeof(T) * length < mNode->mSize && mNode->mReferenceCount == 1)
        {
            Utils::strcat(mBuffer, source);
            mNode->mLength = (uint16) length;
        }
        else
        {
            StringPool::PoolNode* oldNode = mNode;
            T* oldBuffer = mBuffer;

            mNode = StringPool::getSingleton().allocate(sizeof(T) * length);
            mBuffer = (T*) StringPool::getSingleton().getBufferPtr(mNode);
            mNode->mLength = (uint16) length;

            Utils::strcpy(mBuffer, oldBuffer);
            Utils::strcat(mBuffer, source);

            StringPool::getSingleton().free(oldNode);
        }
    }

    template <typename T, T end>
    void DynamicString<T,end>::operator+=(const DynamicString &source)
    {
        if (sizeof(T) * (source.mNode->mLength + mNode->mLength) < mNode->mSize && mNode->mReferenceCount == 1)
        {
            Utils::strcat(mBuffer, source.mBuffer);
            mNode->mLength += source.mNode->mLength;
        }
        else
        {
            uint16 length = mNode->mLength + source.mNode->mLength;

            StringPool::PoolNode* oldNode = mNode;
            T* oldBuffer = mBuffer;

            mNode = StringPool::getSingleton().allocate(sizeof(T) * length);
            mBuffer = (T*) StringPool::getSingleton().getBufferPtr(mNode);
            mNode->mLength = length;

            Utils::strcpy(mBuffer, oldBuffer);
            Utils::strcat(mBuffer, source.mBuffer);

            StringPool::getSingleton().free(oldNode);
        }
    }

    template <typename T, T end>
    DynamicString<T,end>& DynamicString<T,end>::operator=(const T *source)
    {
        uint32 length = Utils::strlen(source);

        StringPool::getSingleton().free(mNode);
        mNode = StringPool::getSingleton().allocate(sizeof(T) * length);
        mBuffer = (T*) StringPool::getSingleton().getBufferPtr(mNode);

        Utils::strcpy(mBuffer, source);
        mNode->mLength = (uint16) length;

        return *this;
    }

    template <typename T, T end>
    DynamicString<T,end>& DynamicString<T,end>::operator=(const DynamicString &source)
    {
        StringPool::getSingleton().free(mNode);
        mNode = source.mNode;
        mBuffer = source.mBuffer;
        mNode->mReferenceCount += 1;

        return *this;
    }

    template <typename T, T end>
    uint32 DynamicString<T,end>::length() const
    {
        return mNode->mLength;
    }

    template <typename T, T end>
    uint32 DynamicString<T,end>::capacity() const
    {
        return mNode->mSize;
    }

    template <typename T, T end>
    uint32 DynamicString<T,end>::referenceCount() const
    {
        return mNode->mReferenceCount;
    }

    template <typename T, T end>
    T* DynamicString<T,end>::get() const
    {
        return mBuffer;
    }

    } // namespace Berserk

#endif //BERSERK_DYNAMICSTRING_H