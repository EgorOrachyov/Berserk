//
// Created by Egor Orachyov on 25.06.2019.
//

#ifndef BERSERK_STRINGSTATIC_H
#define BERSERK_STRINGSTATIC_H

#include <Strings/StringUtility.h>

namespace Berserk
{

    /**
     * Generic const size string with buffer inside. Should be used
     * only for temporal stack operations.
     * @note Thread-Safe, no internal memory allocations
     *
     * @tparam Char Type of characters in string
     * @tparam end End symbol (where the string actually ends)
     * @tparam length Buffer size of the string to store content
     */
    template <typename Char, Char end, uint32 length>
    class ENGINE_API StringStatic
    {
    public:

        /** Utility for this concrete type of static string */
        typedef StringUtility<Char, end> Utility;

        /** Total size of buffer for this static string */
        static const uint32 TOTAL_BUFFER_SIZE = length;

    public:

        StringStatic()
        {
            mBuffer[0] = end;
        }

        StringStatic(const Char* source)
        {
            Utility::copy(mBuffer, TOTAL_BUFFER_SIZE, source);
        }

        StringStatic(const StringStatic& source) = default;

        StringStatic& operator=(const Char* source)
        {
            Utility::copy(mBuffer, TOTAL_BUFFER_SIZE, source);
            return *this;
        }

        StringStatic& operator+=(const Char* source)
        {
            Utility::concat(mBuffer, TOTAL_BUFFER_SIZE, source);
            return *this;
        }

        StringStatic& operator=(const StringStatic& source)
        {
            Utility::copy(mBuffer, source.mBuffer);
            return *this;
        }

        StringStatic& operator+=(const StringStatic& source)
        {
            Utility::concat(mBuffer, TOTAL_BUFFER_SIZE, source.mBuffer);
            return *this;
        }

        StringStatic operator+(const StringStatic& other) const
        {
            StringStatic result = *this;
            result += other;
            return result;
        }

        const bool operator==(const StringStatic& other)
        {
            return Utility::compare(mBuffer, other.mBuffer) == 0;
        }

        const bool operator>(const StringStatic& other) const
        {
            return Utility::compare(mBuffer, other.mBuffer) > 0;
        }

        const bool operator>=(const StringStatic& other) const
        {
            return Utility::compare(mBuffer, other.mBuffer) >= 0;
        }

        const bool operator<(const StringStatic& other) const
        {
            return Utility::compare(mBuffer, other.mBuffer) < 0;
        }

        const bool operator<=(const StringStatic& other) const
        {
            return Utility::compare(mBuffer, other.mBuffer) <= 0;
        }

        uint32 length() const
        {
            return Utility::length(mBuffer);
        }

        const Char* get() const
        {
            return mBuffer;
        }

    protected:

        /** Content of the string */
        Char mBuffer[TOTAL_BUFFER_SIZE];

    };

} // namespace Berserk

#endif //BERSERK_STRINGSTATIC_H