//
// Created by Egor Orachyov on 25.06.2019.
//

#ifndef BERSERK_STRINGDYNAMIC_H
#define BERSERK_STRINGDYNAMIC_H

#include <Misc/Crc32.h>
#include <Strings/StringUtility.h>
#include <Strings/StringManager.h>
#include <Misc/AssertDev.h>
#include <Containers/TArray.h>

namespace Berserk
{

    /**
     * Dynamic string of char characters in c-style with global storage.
     * Strings could be created and could be used in any thread.
     *
     * @warning Concrete string should be process only in one thread. You cannot share the same
     *          string among other threads.
     */
    class ENGINE_API StringDynamic
    {
    private:

        /** Utility for this concrete type of string */
        typedef StringUtility<char, '\0'> Utility;

        /** Manager for this strings char */
        static StringManager& manager;

    public:

        /** Initialize as empty '' string */
        StringDynamic()
                : mInfo(manager.emptyNode()), mBuffer((char*)mInfo->buffer())
        {

        }

        /** Initialize from raw c-style source string */
        StringDynamic(const char* source)
        {
            uint32 size = Utility::length(source) + 1;
            Info* info = manager.createNode(size);
            mInfo = info;
            mInfo->setLenght(size - 1);
            mBuffer = (char*) info->buffer();
            Utility::copy(mBuffer, source);
        }

        /** Initialize from raw c-style source string and length of the string to copy (without '\0' symbol) */
        StringDynamic(const char* source, uint32 length)
        {
            uint32 size = length + 1;
            Info* info = manager.createNode(size);
            mInfo = info;
            mInfo->setLenght(size - 1);
            mBuffer = (char*) info->buffer();
            Utility::copy(mBuffer, size, source);
        }

        /** Initialize from other string */
        StringDynamic(const StringDynamic& source)
                : mInfo(source.mInfo), mBuffer(source.mBuffer)
        {
            manager.incReferences(mInfo);
        }

        /** Initialize from other string */
        StringDynamic(const StringDynamic&& source) noexcept
                : mInfo(source.mInfo), mBuffer(source.mBuffer)
        {
            manager.incReferences(mInfo);
        }

        ~StringDynamic()
        {
            if (mInfo != nullptr)
            {
                manager.deleteNode(mInfo);
                mInfo = nullptr;
                mBuffer = nullptr;
            }
        }

        StringDynamic& operator=(const StringDynamic& source)
        {
            manager.deleteNode(mInfo);
            mInfo = source.mInfo;
            mBuffer = source.mBuffer;
            manager.incReferences(mInfo);
            return *this;
        }

        StringDynamic& operator=(const char* source)
        {
            uint32 size = Utility::length(source) + 1;
            Info* info = manager.createNode(size);
            manager.deleteNode(mInfo);
            mInfo = info;
            mInfo->setLenght(size - 1);
            mBuffer = (char*) mInfo->buffer();
            Utility::copy(mBuffer, source);
            return *this;
        }

        StringDynamic& operator+=(const StringDynamic& source)
        {
            uint32 size = length() + source.length() + 1;
            Info* info = manager.createNode(size);
            Utility::copy((char*)info->buffer(), mBuffer);
            Utility::concat((char*)info->buffer(), source.mBuffer);
            manager.deleteNode(mInfo);
            mInfo = info;
            mInfo->setLenght(size - 1);
            mBuffer = (char*) mInfo->buffer();
            return *this;
        }

        StringDynamic& operator+=(const char* source)
        {
            uint32 size = length() + Utility::length(source) + 1;
            Info* info = manager.createNode(size);
            Utility::copy((char*)info->buffer(), mBuffer);
            Utility::concat((char*)info->buffer(), source);
            manager.deleteNode(mInfo);
            mInfo = info;
            mInfo->setLenght(size - 1);
            mBuffer = (char*) mInfo->buffer();
            return *this;
        }

        StringDynamic operator+(const StringDynamic& source) const
        {
            uint32 size = length() + source.length() + 1;
            Info* info = manager.createNode(size);
            Utility::copy((char*)info->buffer(), mBuffer);
            Utility::concat((char*)info->buffer(), source.mBuffer);

            StringDynamic string;
            manager.deleteNode(string.mInfo);
            string.mInfo = info;
            string.mInfo->setLenght(size - 1);
            string.mBuffer = (char*) info->buffer();

            return string;
        }

        StringDynamic operator+(const char* source) const
        {
            uint32 size = length() + Utility::length(source) + 1;
            Info* info = manager.createNode(size);
            Utility::copy((char*)info->buffer(), mBuffer);
            Utility::concat((char*)info->buffer(), source);

            StringDynamic string;
            manager.deleteNode(string.mInfo);
            string.mInfo = info;
            string.mInfo->setLenght(size - 1);
            string.mBuffer = (char*) info->buffer();

            return string;
        }

        StringDynamic substring(uint32 from, uint32 length) const
        {
            assertion_dev_msg(from + length <= StringDynamic::length(),
                              "StringDynamic: '%s' refs: %u", get(), mInfo->references());

            StringDynamic string;
            Info* info = manager.createNode(length + 1);
            Utility::substring((char*)info->buffer(), mBuffer, from, length);
            manager.deleteNode(string.mInfo);
            string.mInfo = info;
            string.mBuffer = (char*) info->buffer();
            string.mInfo->setLenght(length);

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
            return mInfo->length();
        }

        uint32 size() const
        {
            return mInfo->size();
        }

        const char* get() const
        {
            return mBuffer;
        }

        char* get()
        {
            return mBuffer;
        }

        /**
         * Hashing with crc32 algorithm of the string content
         * @param string Source to hash
         * @return Hashed value of the string buffer
         */
        static uint32 hash(const StringDynamic& string)
        {
            return Crc32::hash(string.get(), string.length());
        }

        /**
         * Convert int value to string
         * @param value To convert
         * @return String representation of the value
         */
        static StringDynamic toString(int32 value)
        {
            char buffer[16];
            snprintf(buffer, 16, "%i", value);

            return StringDynamic(buffer);
        }

        /**
         * Convert float value to string
         * @param value To convert
         * @return String representation of the value
         */
        static StringDynamic toString(float32 value)
        {
            char buffer[16];
            snprintf(buffer, 16, "%f", value);

            return StringDynamic(buffer);
        }

        /**
         * Convert string value to int
         * @param string Source to convert
         * @return Value
         */
        static int32 toInt32(const char* string)
        {
            int32 value;
            int32 read = sscanf(string, "%i", &value);
            assertion_dev_msg(read == 1, "Cannot convert [string: %s] to value", string);

            return atoi(string);
        }

        /**
         * Convert string value to float
         * @param string Source to convert
         * @return Value
         */
        static float32 toFloat32(const char* string)
        {
            int32 value;
            int32 read = sscanf(string, "%i", &value);
            assertion_dev_msg(read == 1, "Cannot convert [string: %s] to value", string);

            return (float32) atof(string);
        }

        /**
         * Split specified string via symbols and save result in the out array
         *
         * @example string = "a bsd=ae ,a"
         *          symbols = " =,"
         *          out = { "a", "bsd", "ae", "a" }
         *
         * @param string Source to split
         * @param symbols Buffer with symbols which used to define, which symbols to ignore
         * @param out To save split strings
         */
        static void split(const char* string, const char* symbols, TArray<StringDynamic> &out)
        {
            const char* toSplit = string;
            uint32 toSpiltLength = 0;

            while (*string != '\0')
            {
                bool ignoreCurrentChar = false;
                const char* ignore = symbols;
                while (*ignore != '\0' && !ignoreCurrentChar)
                {
                    if (*ignore == *string)
                    {
                        ignoreCurrentChar = true;
                    }

                    ignore += 1;
                }

                if (ignoreCurrentChar)
                {
                    if (toSpiltLength > 0)
                    {
                        out.add(StringDynamic(toSplit, toSpiltLength));
                        toSpiltLength = 0;
                    }
                }
                else
                {
                    if (toSpiltLength == 0)
                    {
                        toSplit = string;
                    }
                    toSpiltLength += 1;
                }

                string += 1;
            }

            if (toSpiltLength > 0)
            {
                out.add(StringDynamic(toSplit, toSpiltLength));
            }
        }

    private:

        /** Content of the string, stored in the memory manager */
        typedef StringManager::StringInfo Info;

        /** Buffer, length, references */
        Info* mInfo = nullptr;

        /** Buffer for fast access */
        char* mBuffer = nullptr;

    };

} // namespace Berserk

#endif //BERSERK_STRINGDYNAMIC_H