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
#include <Serialization/ArchiveReader.h>
#include <Serialization/ArchiveWriter.h>

namespace Berserk
{

    /**
     * Dynamic string of char characters in c-style with global storage.
     * Strings could be created and could be used in any thread.
     *
     * @note Strings could be created from any thread
     *
     * @warning Concrete string should be process only in one thread. You cannot share the same
     *          string among other threads.
     */
    class ENGINE_API StringDynamic
    {
    protected:

        /** Utility for this concrete type of string */
        typedef StringUtility<char, '\0'> Utility;

        /** Content of the string, stored in the memory manager */
        typedef StringManager::StringInfo Info;

        /** Manager for this strings char */
        static StringManager& manager;

        /** For wrapping */
        StringDynamic(Info* info, const char* source)
        {
            mInfo = info;
            mBuffer = (char*) source;
        }

        /** Pre-allocates empty string buffer*/
        explicit StringDynamic(uint32 length)
        {
            uint32 size = length + 1;
            Info* info = manager.createNode(size);
            mInfo = info;
            mInfo->setLenght(length);
            mBuffer = (char*) info->buffer();
        }

    public:

        /** Initialize as empty '' string */
        StringDynamic()
            : mInfo(getNullStringInfo()), mBuffer(getNullStringBuffer())
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
        StringDynamic(StringDynamic&& source) noexcept
                : mInfo(source.mInfo), mBuffer(source.mBuffer)
        {
            source.mInfo = getNullStringInfo();
            source.mBuffer = getNullStringBuffer();
        }

        ~StringDynamic()
        {
            if (!isNullString())
            {
                manager.deleteNode(mInfo);

                mInfo = getNullStringInfo();
                mBuffer = getNullStringBuffer();
            }
        }

        StringDynamic& operator=(const StringDynamic& source)
        {
            deleteInfo();
            mInfo = source.mInfo;
            mBuffer = source.mBuffer;
            manager.incReferences(mInfo);
            return *this;
        }

        StringDynamic& operator=(const char* source)
        {
            uint32 size = Utility::length(source) + 1;
            Info* info = manager.createNode(size);
            deleteInfo();
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
            deleteInfo();
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
            deleteInfo();
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
            string.mInfo = info;
            string.mBuffer = (char*) info->buffer();
            string.mInfo->setLenght(length);

            return string;
        }

        /** @return Lower case converted string */
        StringDynamic toLowerCase() const
        {
            StringDynamic str(length());
            char* buffer = str.mBuffer;

            for (uint32 i = 0; i < length(); i++)
            {
                buffer[i] = (char) tolower(mBuffer[i]);
            }

            buffer[length()] = '\0';

            return str;
        }

        const bool operator!=(const StringDynamic& other) const
        {
            return Utility::compare(mBuffer, other.mBuffer) != 0;
        }

        const bool operator==(const StringDynamic& other) const
        {
            return Utility::compare(mBuffer, other.mBuffer) == 0;
        }

        const bool operator==(const char* other) const
        {
            return Utility::compare(mBuffer, other) == 0;
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

        /**
         * Serialize raw dynamic string content to the archive
         * @param archive Archive to save string content
         * @param string String to serialize
         * @return archive
         */
        friend ArchiveWriter& operator<< (ArchiveWriter& archive, const StringDynamic& string)
        {
            uint32 length = string.length();

            archive << length;
            archive.serialize(string.get(), length);

            return archive;
        }

        /**
         * Deserialize string from archive, previously created by operator<< call
         * @param archive Archive to read string content
         * @param string String to save result
         * @return archive
         */
        friend ArchiveReader& operator>> (ArchiveReader& archive, StringDynamic& string)
        {
            uint32 length = 0;

            StringDynamic result(length);

            archive >> length;
            archive.deserialize(result.get(), length);

            result.get()[length] = '\0';
            string = result;

            return archive;
        }

    protected:

        void deleteInfo() const
        {
            if (!isNullString()) manager.deleteNode(mInfo);
        }

        bool isNullString() const
        {
            return mInfo == getNullStringInfo() || mBuffer == getNullStringBuffer();
        }

        static Info* getNullStringInfo()
        {
            static Info info;
            return &info;
        }

        static char* getNullStringBuffer()
        {
            static char buffer[] = { '\0' };
            return buffer;
        }

    protected:

        /** Buffer, length, references */
        Info* mInfo = nullptr;

        /** Buffer for fast access */
        char* mBuffer = nullptr;

    };

    /** String wrapper for const char sources created on stack */
    class StringDynamic_Wrapper : public StringDynamic
    {
    public:

        /** Wrap C-style const string */
        StringDynamic_Wrapper(const char* source) : StringDynamic(&mInfoSource, source)
        {
            uint32 length = Utility::length(source);
            mInfo->setLenght(length);
            mInfo->incReference();
        }

        ~StringDynamic_Wrapper()
        {
            mInfo = getNullStringInfo();
            mBuffer = getNullStringBuffer();
        }

    private:

        /** Info for this wrapped string */
        Info mInfoSource;

    };

} // namespace Berserk

#endif //BERSERK_STRINGDYNAMIC_H