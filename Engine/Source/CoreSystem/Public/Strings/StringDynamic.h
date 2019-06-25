//
// Created by Egor Orachyov on 25.06.2019.
//

#ifndef BERSERK_STRINGDYNAMIC_H
#define BERSERK_STRINGDYNAMIC_H

#include <Strings/StringUtility.h>
#include <Strings/StringManager.h>

namespace Berserk
{

    /**
     * Dynamic string of char characters in c-style with global storage.
     * Strings could be created and could be used in any thread.
     *
     * Concrete string should be process only in one thread. You cannot share the same
     * string among other string.
     */
    class ENGINE_API StringDynamic
    {
    private:

        /** Utility for this concrete type of string */
        typedef StringUtility<char, '\0'> Utility;

        /** Manager for this strings char */
        static StringManager& manager;

    public:

        StringDynamic()
                : mInfo(manager.emptyNode()), mBuffer((char*)mInfo.buffer())
        {

        }

        StringDynamic(const char* source)
                : mInfo(manager.createNode(Utility::length(source))), mBuffer((char*)mInfo.buffer())
        {
            Utility::copy(mBuffer, source);
        }

        StringDynamic(const StringDynamic& source)
                : mInfo(source.mInfo), mBuffer((char*)mInfo.buffer())
        {
            manager.incReferences(mInfo);
        }

        StringDynamic(const StringDynamic&& source) noexcept
                : mInfo(source.mInfo), mBuffer((char*)mInfo.buffer())
        {
            manager.incReferences(mInfo);
        }

        ~StringDynamic()
        {
            manager.deleteNode(mInfo);
            mBuffer = nullptr;
        }

        StringDynamic& operator=(const StringDynamic& source)
        {
            manager.deleteNode(mInfo);
            mInfo = source.mInfo;
            mBuffer = (char*) mInfo.buffer();
            manager.incReferences(mInfo);
            return *this;
        }

        StringDynamic& operator+=(const StringDynamic& source)
        {
            uint32 size = length() + source.length() + 1;
            Info& info = manager.createNode(size);
            Utility::copy((char*)info.buffer(), mBuffer);
            Utility::concat((char*)info.buffer(), source.mBuffer);
            manager.deleteNode(mInfo);
            mInfo = info;
            mBuffer = (char*) mInfo.buffer();
            return *this;
        }

        StringDynamic operator+(const StringDynamic& source)
        {
            uint32 size = length() + source.length() + 1;
            Info& info = manager.createNode(size);
            Utility::copy((char*)info.buffer(), mBuffer);
            Utility::concat((char*)info.buffer(), source.mBuffer);

            StringDynamic string;
            manager.deleteNode(string.mInfo);
            string.mInfo = info;
            string.mBuffer = (char*) info.buffer();

            return string;
        }

        const bool operator!=(const StringDynamic& other) const
        {
            return Utility::compare(mBuffer, other.mBuffer) != 0;
        }

        const bool operator==(const StringDynamic& other) const
        {
            return Utility::compare(mBuffer, other.mBuffer) == 0;
        }

        const bool operator>(const StringDynamic& other) const
        {
            return Utility::compare(mBuffer, other.mBuffer) > 0;
        }

        const bool operator>=(const StringDynamic& other) const
        {
            return Utility::compare(mBuffer, other.mBuffer) >= 0;
        }

        const bool operator<(const StringDynamic& other) const
        {
            return Utility::compare(mBuffer, other.mBuffer) < 0;
        }

        const bool operator<=(const StringDynamic& other) const
        {
            return Utility::compare(mBuffer, other.mBuffer) <= 0;
        }

        uint32 length() const
        {
            return mInfo.length();
        }

        uint32 size() const
        {
            return mInfo.size();
        }

        const char* get() const
        {
            return mBuffer;
        }

        char* get()
        {
            return mBuffer;
        }

    private:

        /** Content of the string, stored in the memory manager */
        typedef StringManager::StringInfo Info;

        /** Buffer, length, references */
        Info& mInfo;

        /** Buffer for fast access */
        char* mBuffer = nullptr;

    };

    /** Reference to the global char string manager */
    StringManager& StringDynamic::manager = StringManager::getCharStringManager();

} // namespace Berserk

#endif //BERSERK_STRINGDYNAMIC_H