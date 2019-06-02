//
// Created by Egor Orachyov on 29.01.2019.
//

#ifndef BERSERK_DYNAMICSTRING_H
#define BERSERK_DYNAMICSTRING_H

#include <Misc/Assert.h>
#include <Object/NewDelete.h>
#include <Logging/LogMacros.h>
#include <Strings/StringPool.h>
#include <Strings/StringUtility.h>

namespace Berserk
{
    /** Add additional info for strings usage */

    #ifndef PROFILE_DYNAMIC_STRING
        #define PROFILE_DYNAMIC_STRING 0
    #endif // PROFILE_DYNAMIC_STRING

    /**
     * Dynamic string which allocates memory in string pool for its data.
     * Supports reference counting and O(1) copy with ref++.
     * Allows create instant strings, modify strings with creating new instances.
     *
     * @tparam T    Type of character
     * @tparam end  End symbol for string ('\0', L'\0')
     */
    template <typename T, T end>
    class DynamicString
    {
    public:

        /** Null string (no memory usage) */
        DynamicString();

        /** Empty string with chosen size */
        DynamicString(uint32 size);

        /** String from source */
        explicit DynamicString(const T* source);

        /** Copy constructor */
        DynamicString(const DynamicString& source);

        /** Release acquired memory */
        ~DynamicString();

        GENERATE_NEW_DELETE(DynamicString);

    public:

        void nullify()
        {
            mNode = nullptr;
            mBuffer = nullptr;
        }

        /** Nullify string (no memory usage) */
        void empty();

        /** Create instant string from this with 1 ref count */
        void instant();

        /** Append string with source sting */
        void operator += (const T *source);

        /** Append string with source sting */
        void operator += (const DynamicString& source);

        /** Assign source to this string */
        DynamicString& operator = (const T *source);

        /** Assign source to this string (immutability with ref++) */
        DynamicString& operator = (const DynamicString& source);

        /** @return True if equal */
        bool operator==(const T* source);

        /** @return True if equal */
        bool operator==(const DynamicString& source);

        /** @return Length of string */
        uint32 length() const;

        /** @return Max chars in string without expanding */
        uint32 capacity() const;

        /** @return Num of references to this PoolNode (string) */
        uint32 referenceCount() const;

        /** @return Pointer to raw data */
        T* get() const;

        /** @return Max supported length for this type of string */
        static uint32 maxLength();

    private:

        typedef Strings<T, end> Utils;

        T* mBuffer;                     // String data (Raw chars)
        StringPool::PoolNode* mNode;    // String info (Pointer to struct in pool)

    };

    template <typename T, T end>
    DynamicString<T,end>::DynamicString()
    {
        mNode = StringPool::getSingleton().create();
        mBuffer = (T*) StringPool::getSingleton().getBufferPtr(mNode);
    }

    template <typename T, T end>
    DynamicString<T,end>::DynamicString(uint32 size)
    {
        mNode = StringPool::getSingleton().allocate(sizeof(T) * size);
        mBuffer = (T*) StringPool::getSingleton().getBufferPtr(mNode);
        mBuffer[0] = end;
    }

    template <typename T, T end>
    DynamicString<T,end>::DynamicString(const T *source)
    {
        FAIL(source, "Null pointer source string");

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
            #if PROFILE_DYNAMIC_STRING
                PUSH("DynamicString: delete [string: '%s'][ref: %u]", (char*)mBuffer, mNode->mReferenceCount);
            #endif

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
        mBuffer = (T*) StringPool::getSingleton().getBufferPtr(mNode);
        mNode->mLength = oldNode->mLength;

        Utils::strcpy(mBuffer, oldBuffer);

        StringPool::getSingleton().free(oldNode);
    }

    template <typename T, T end>
    void DynamicString<T,end>::operator+=(const T *source)
    {
        ASSERT(source, "Null pointer source string");

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
        ASSERT(source, "Null pointer source string");

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
    bool DynamicString<T,end>::operator==(const T *source)
    {
        ASSERT(source, "Null pointer source string");

        if (mNode->mSize == 0)
        {
            return false;
        }

        return (Utils::strcmp(mBuffer, source) == 0);
    }

    template <typename T, T end>
    bool DynamicString<T,end>::operator==(const DynamicString &source)
    {
        if (mNode == source.mNode)
        {
            return true;
        }

        if (mNode->mSize == 0)
        {
            return false;
        }

        return (Utils::strcmp(mBuffer, source.mBuffer) == 0);
    }

    template <typename T, T end>
    uint32 DynamicString<T,end>::length() const
    {
        return mNode->mLength;
    }

    template <typename T, T end>
    uint32 DynamicString<T,end>::capacity() const
    {
        return mNode->mSize / sizeof(T);
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

    template <typename T, T end>
    uint32 DynamicString<T,end>::maxLength()
    {
        return StringPool::MAX_BUFFER_SIZE / sizeof(T);
    }

    } // namespace Berserk

#endif //BERSERK_DYNAMICSTRING_H