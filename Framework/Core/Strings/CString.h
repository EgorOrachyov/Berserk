//
// Created by Egor Orachyov on 03.06.2018.
//

#ifndef BERSERKENGINE_CSTRING_H
#define BERSERKENGINE_CSTRING_H

#include "Essential/Types.h"
#include "Essential/Common.h"
#include "Essential/UsageDescriptors.h"

#include "CStaticString.h"

namespace Berserk
{

    class CString
    {
    public:

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param size Length of string without last terminate symbol
         * @param charsBuffer Pointer to chars buffer to be copied
         */
        CString(UINT32 size, const CHAR* charsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param charsBuffer Pointer to chars buffer to be copied
         */
        CString(const CHAR* charsBuffer);

        /**
         * Returns used by string memory block to string buffer
         */
        ~CString();

        /**
         * Set empty string with 0 length
         */
        void empty();

        /**
         * By char coping of source string
         *
         * @param source String to be copied
         */
        void copy(CString &source);

        /**
         *
         * @param source
         */
        void copy(CStaticString &source);

        /**
         * Writes source in the back of string while it has not used place
         *
         * @param source String to be appended in the back
         */
        void append(CString &source);

        /**
         *
         * @param source
         */
        void append(CStaticString &source);

        /**
         * Finds first substring in the target
         *
         * @param subString To be found
         * @return Offset to found string of NOT_FOUND flag
         */
        UINT32 find(CString &subString);

        /**
         * Finds first substring in the target
         *
         * @param subString To be found
         * @return Offset to found string of NOT_FOUND flag
         */
        UINT32 find(CStaticString &subString);

        /**
         * Finds first char in the target
         *
         * @param symbol To be found
         * @return Offset to found symbol of NOT_FOUND flag
         */
        UINT32 find(CHAR symbol);

        /**
         * Get size of string (without termination symbol)
         *
         * @return Current size
         */
        UINT32 getSize();

        /**
         * Get max size of string (its capacity) without termination symbol
         *
         * @return Max capacity (node: this string cannot be expanded)
         */
        UINT32 getCapacity();

        /**
         * Is this symbol in the string
         *
         * @param symbol To be checked
         * @return FOUND or NOT_FOUND flags
         */
        INT32 contains(CHAR symbol);

        /**
         * Type of string (@see StringType)
         *
         * @return Type of this string
         */
        INT32 getType();

        /**
         *
         * Get pointer to standard ASCII string
         *
         * @return CHARS* pointer to buffer
         */
        const CHAR* getCharsBuffer();

        /**
         * Assignment (copy) operator
         * @param source
         * @return This assigned string
         */
        CString operator = (const CString& source);

        /**
         * Append string
         * @param source
         * @return this + staticString
         */
        CString operator + (const CString& source) const;

        /**
         * Append string
         * @param source
         * @return this + staticString
         */
        CString operator + (const CStaticString& source) const;

        /**
         * Append symbol
         * @param staticString
         * @return this + c
         */
        CString operator + (CHAR c) const;

        /**
         * By symbol strings comparing
         *
         * @param source to compare
         * @return true if strings are equal
         */
        const bool operator == (const CString& source) const;

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
        UINT16 mCapacity;               // Max available length without '\0'
        CHAR*  mBuffer;                 // Pointer to chars buffer in the strings bank

        
    };

} // namespace Berserk

#endif //BERSERKENGINE_CSTRING_H