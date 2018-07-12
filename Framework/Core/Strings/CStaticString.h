//
// Created by Egor Orachyov on 09.06.2018.
//

#ifndef BERSERKENGINE_CSTATICSTRING_H
#define BERSERKENGINE_CSTATICSTRING_H

#include "Essential/Types.h"
#include "Essential/Common.h"
#include "Essential/UsageDescriptors.h"

namespace Berserk
{

    /**
     * Static size string, which can be created at once with fixed capacity
     * and then used as simple string with limitation on size.
     * Encoding format is ASCII.
     *
     * Note: all operations are limited by capacity of the string, therefore
     * be ready to get cut strings
     */
    class DATA_API CStaticString
    {
    public:

        /**
         * String initializing (empty string) by simply passing essential
         * params (could be used without this initialization)
         */
        CStaticString();

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param size Length of string without last terminate symbol
         * @param charsBuffer Pointer to chars buffer to be copied
         */
        CStaticString(UINT32 size, const CHAR* charsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param charsBuffer Pointer to chars buffer to be copied
         */
        CStaticString(const CHAR* charsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param size Length of string without last terminate symbol
         * @param charsBuffer Pointer to chars buffer to be copied
         */
        void init(UINT32 size, const CHAR *charsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param charsBuffer Pointer to chars buffer to be copied
         */
        void init(const CHAR *charsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param anotherString String to be copied
         */
        void init(CStaticString &anotherString);

        /**
         * Set empty string with 0 length
         */
        void empty();

        /**
         * By char coping of source string
         *
         * @param source String to be copied
         */
        void copy(CStaticString &source);

        /**
         * Write mask in target string and insert source in positions marked
         * by symbols %s (works as simple printf)
         *
         * @param source String to placed in %s positions
         * @param mask Writing template
         */
        void copy(CStaticString &source, CStaticString &mask);

        /**
         * Copy from ASCII string count of characters (while has empty space)
         *
         * @param source Pointer to ASCII string
         * @param count Num of chars to be copied
         */
        void copy(const CHAR *source, UINT32 count);

        /**
         * Copy from ASCII string (while has empty space)
         *
         * @param source Pointer to ASCII string
         */
        void copy(const CHAR *source);

        /**
         * Writes source in the back of string while it has not used place
         *
         * @param source String to be appended in the back
         */
        void append(CStaticString &source);

        /**
         * Appends count chars by using ASCII string
         *
         * @param source Pointer to ASCII string
         * @param count Num of chars to be appended
         */
        void append(const CHAR *source, UINT32 count);

        /**
         * Appends chars by using ASCII string
         *
         * @param source Pointer to ASCII string
         */
        void append(const CHAR *source);

        /**
         * Append ASCII symbol in the end of string
         *
         * @param symbol To be added
         */
        void append(CHAR symbol);

        /**
         * Inserts substring source from the offset position
         *
         * @param source To be inserted
         * @param offset Start index of insertion (0 - insert from beginning, target length -
         *        insert from the end)
         */
        void insert(CStaticString &source, UINT32 offset);

        /**
         * Finds first substring in the target
         *
         * @param subString To be found
         * @return Offset to found string of NOT_FOUND flag
         */
        UINT32 find(CStaticString &subString) const;

        /**
         * Finds first char in the target
         *
         * @param symbol To be found
         * @return Offset to found symbol of NOT_FOUND flag
         */
        UINT32 find(CHAR symbol) const;

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
        INT32 contains(CHAR symbol) const;

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
        const CHAR* getChars() const;

        /**
         * Assignment (copy) operator
         * @param staticString
         * @return This assigned string
         */
        CStaticString operator = (const CStaticString& staticString);

        /**
         * Append string
         * @param staticString
         * @return this + staticString
         */
        CStaticString operator + (const CStaticString& staticString) const;

        /**
         * Append symbol
         * @param staticString
         * @return this + c
         */
        CStaticString operator + (CHAR c) const;

        /**
         * By symbol strings comparing
         *
         * @param staticString to compare
         * @return true if strings are equal
         */
        const bool operator == (const CStaticString& staticString) const;

    private:

        friend class CString;

        UINT32 mSize;                       // Current length without '\0' symbol
        UINT32 mCapacity;                   // Max available size
        CHAR mBuffer[BUFFER_SIZE_128];      // Buffer

    };

} // namespace Berserk

#endif //BERSERKENGINE_CSTATICSTRING_H
