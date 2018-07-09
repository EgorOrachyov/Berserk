//
// Created by Egor Orachyov on 26.06.2018.
//

#ifndef BERSERKENGINE_STATICSTRINGUTF32_H
#define BERSERKENGINE_STATICSTRINGUTF32_H

#include "../Essential/Types.h"
#include "../Essential/Common.h"
#include "../Essential/UsageDescriptors.h"

namespace Berserk
{

    /**
     * Static size string, which can be created at once with fixed capacity
     * and then used as simple string with limitation on size.
     * Encoding format is UTF32;
     *
     * Note: all operations are limited by capacity of the string, therefore
     * be ready to get cut strings
     */
    class DATA_API WCAHRStaticString
    {
    public:

        /**
         * String initializing (empty string) by simply passing essential
         * params (could be used without this initialization)
         */
        WCAHRStaticString();

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param size Length of string without last terminate symbol
         * @param wcharsBuffer Pointer to wchars buffer to be copied
         */
        WCAHRStaticString(UINT32 size, const WCHAR* wcharsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param wcharsBuffer Pointer to wchars buffer to be copied
         */
        WCAHRStaticString(const WCHAR* wcharsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param size Length of string without last terminate symbol
         * @param wcharsBuffer Pointer to wchars buffer to be copied
         */
        void init(UINT32 size, const WCHAR *wcharsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param wcharsBuffer Pointer to wchars buffer to be copied
         */
        void init(const WCHAR *wcharsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param anotherString String to be copied
         */
        void init(WCAHRStaticString &anotherString);

        /**
         * Set empty string with 0 length
         */
        void empty();

        /**
         * By wchar coping of source string
         *
         * @param source String to be copied
         */
        void copy(WCAHRStaticString &source);

        /**
         * Write mask in target string and insert source in positions marked
         * by symbols %s (works as simple printf)
         *
         * @param source String to placed in %s positions
         * @param mask Writing template
         */
        void copy(WCAHRStaticString &source, WCAHRStaticString &mask);

        /**
         * Copy from UT32 string count of wcharacters (while has empty space)
         *
         * @param source Pointer to UT32 string
         * @param count Num of wchars to be copied
         */
        void copy(const WCHAR *source, UINT32 count);

        /**
         * Copy from UT32 string (while has empty space)
         *
         * @param source Pointer to UT32 string
         */
        void copy(const WCHAR *source);

        /**
         * Writes source in the back of string while it has not used place
         *
         * @param source String to be appended in the back
         */
        void append(WCAHRStaticString &source);

        /**
         * Appends count wchars by using UT32 string
         *
         * @param source Pointer to UT32 string
         * @param count Num of wchars to be appended
         */
        void append(const WCHAR *source, UINT32 count);

        /**
         * Appends wchars by using UT32 string
         *
         * @param source Pointer to UT32 string
         */
        void append(const WCHAR *source);

        /**
         * Append UT32 symbol in the end of string
         *
         * @param symbol To be added
         */
        void append(WCHAR symbol);

        /**
         * Inserts substring source from the offset position
         *
         * @param source To be inserted
         * @param offset Start index of insertion (0 - insert from beginning, target length -
         *        insert from the end)
         */
        void insert(WCAHRStaticString &source, UINT32 offset);

        /**
         * Finds first substring in the target
         *
         * @param subString To be found
         * @return Offset to found string of NOT_FOUND flag
         */
        UINT32 find(WCAHRStaticString &subString);

        /**
         * Finds first wchar in the target
         *
         * @param symbol To be found
         * @return Offset to found symbol of NOT_FOUND flag
         */
        UINT32 find(WCHAR symbol);

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
        INT32 contains(WCHAR symbol);

        /**
         * Type of string (@see StringType)
         *
         * @return Type of this string
         */
        INT32 getType();

        /**
         *
         * Get pointer to standard UT32 string
         *
         * @return WCHARS* pointer to buffer
         */
        const WCHAR* getCharsBuffer();

    private:

        UINT32 mSize;                       // Current length without L'\0' symbol
        UINT32 mCapacity;                   // Max available size
        WCHAR mBuffer[BUFFER_SIZE_128];     // Buffer

    };

} // namespace Berserk

#endif //BERSERKENGINE_STATICSTRINGUTF32_H
