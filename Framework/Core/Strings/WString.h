//
// Created by Egor Orachyov on 03.06.2018.
//

#ifndef BERSERKENGINE_WSTRING_H
#define BERSERKENGINE_WSTRING_H

#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"

#include "WStaticString.h"

namespace Berserk
{

    /**
     * Dynamically resizeable hashed wchar string. Allow to create strings with sizes (basically
     * power of 2) from 32 to 1024. Stores all the data in the global hidden string buffer.
     * Resize automatically in its methods if need.
     *
     * @note Try to not create empty strings
     * @note All the strings are hashed to be stored in the global string table
     * @note If strings sizes are suitable the internal buffer won't be upscaled
     *
     * @warning Pass this strings in functions via pointers or links
     * @warning Use without new operator
     * @warning Strings with length more than 1024 will be cut down
     */
    class DATA_API WString
    {
    public:

        /**
         * Init empty string with 0 length
         */
        WString();

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param size Length of string without last terminate symbol
         * @param charsBuffer Pointer to chars buffer to be copied
         */
        WString(UINT32 size, const WCHAR* charsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param charsBuffer Pointer to chars buffer to be copied
         */
        WString(const WCHAR* charsBuffer);

        /**
         * Returns used by string memory block to string buffer
         */
        ~WString();

        /**
         * Set empty string with 0 length
         */
        void empty();

        /**
         * By wchar coping of source string
         *
         * @param source String to be copied
         */
        void copy(const WString &source);

        /**
         *
         * @param source
         */
        void copy(const WStaticString &source);

        /**
         * Writes source in the back of string while it has not used place
         *
         * @param source String to be appended in the back
         */
        void append(const WString &source);

        /**
         *
         * @param source
         */
        void append(const WStaticString &source);

        /**
         * Finds first substring in the target
         *
         * @param subString To be found
         * @return Offset to found string of NOT_FOUND flag
         */
        UINT32 find(const WString &subString) const;

        /**
         * Finds first substring in the target
         *
         * @param subString To be found
         * @return Offset to found string of NOT_FOUND flag
         */
        UINT32 find(const WStaticString &subString) const;

        /**
         * Finds first wchar in the target
         *
         * @param symbol To be found
         * @return Offset to found symbol of NOT_FOUND flag
         */
        UINT32 find(WCHAR symbol) const;

        /**
         * Get size of string (without termination symbol)
         *
         * @return Current size
         */
        UINT32 getSize() const;

        /**
         * Get max size of string (its capacity) without termination symbol
         *
         * @return Max capacity (node: this string cannot be expanded)
         */
        UINT32 getCapacity() const;

        /**
         * Is this symbol in the string
         *
         * @param symbol To be checked
         * @return FOUND or NOT_FOUND flags
         */
        INT32 contains(WCHAR symbol) const;

        /**
         * Type of string (@see StringType)
         *
         * @return Type of this string
         */
        INT32 getType() const;

        /**
         *
         * Get pointer to standard ASCII string
         *
         * @return CHARS* pointer to buffer
         */
        const WCHAR* getChars() const;

        /**
         * Assignment (copy) operator
         * @param source
         * @return This assigned string
         */
        WString& operator = (const WString& source);

        /**
         * Append string
         * @param source
         * @return this + staticString
         */
        WString& operator += (const WString& source);

        /**
         * Append string
         * @param source
         * @return this + staticString
         */
        WString& operator += (const WStaticString& source);

        /**
         * By symbol strings comparing
         *
         * @param source to compare
         * @return true if strings are equal
         */
        const bool operator == (const WString& source) const;

        /**
         * By symbol strings comparing
         *
         * @param source to compare
         * @return true if strings are equal
         */
        const bool operator == (const WStaticString& source) const;

    private:

        /**
         * Get capable capacity for concrete size
         *
         * @param size Desired size of string
         * @return Capacity needed for buffer
         */
        static UINT16 getCapacity(UINT32 size);

    private:

        UINT32 mStringID;               // Hashed string id for string table
        UINT16 mSize;                   // Current length without '\0'
        UINT16 mCapacity;               // Max available length with '\0'
        WCHAR* mBuffer;                 // Pointer to chars buffer in the strings bank

    };

} // namespace Berserk

#endif //BERSERKENGINE_WSTRING_H