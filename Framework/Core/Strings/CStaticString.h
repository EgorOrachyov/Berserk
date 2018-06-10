//
// Created by Egor Orachyov on 09.06.2018.
//

#ifndef BERSERKENGINE_CSTATICSTRING_H
#define BERSERKENGINE_CSTATICSTRING_H

#include "../Essential/Types.h"
#include "../Essential/UsageDescriptors.h"

namespace Berserk
{

    /**
     * Static size string, which can be created at once with fixed capacity
     * and then used as simple string with limitation on size. Creates by
     * Strings factory functions (common and efficient method)
     *
     * Note: all operations are limited by capacity of the string, therefore
     * be ready to get cut strings
     */
    class CStaticString
    {
    public:

        CStaticString();
        ~CStaticString();

        /**
         * Factory method for string initializing by simply passing essential
         * params (string initializing should be handled by factory method)
         *
         * @param size Length of string without last terminate symbol
         * @param capacity Maximum length of string
         * @param hash Hashed value
         * @param charsBuffer Pointer to allocated chars buffer
         */
        void Init(uint16 size, uint16 capacity, uint32 hash, CHAR* charsBuffer);

        /**
         * Delete string and free internal allocated buffer
         */
        void Delete();

        /**
         * Set empty string with 0 length
         */
        void Empty();

        /**
         * By char coping of source string
         *
         * @param source String to be copied
         */
        void Copy(CStaticString source);

        /**
         * Write mask in target string and insert source in positions marked
         * by symbols %s (works as simple printf)
         *
         * @param source String to placed in %s positions
         * @param mask Writing template
         */
        void Copy(CStaticString source, CStaticString mask);

        /**
         * Copy from ASCII string count of characters (while has empty space)
         *
         * @param source Pointer to ASCII string
         * @param count Num of chars to be copied
         */
        void Copy(CHAR* source, uint16 count);

        /**
         * Writes source in the back of string while it has not used place
         *
         * @param source String to be appended in the back
         */
        void Append(CStaticString source);

        /**
         * Appends count chars by using ASCII string
         *
         * @param source Pointer to ASCII string
         * @param count Num of chars to be appended
         */
        void Append(CHAR* source, uint16 count);

        /**
         * Append ASCII symbol in the end of string
         *
         * @param symbol To be added
         */
        void Append(CHAR symbol);

        /**
         * Inserts substring source from the offset position
         *
         * @param source To be inserted
         * @param offset Start index of insertion (0 - insert from beginning, target length -
         *        insert from the end)
         */
        void Insert(CStaticString source, uint16 offset);

        /**
         * Finds first substring in the target
         *
         * @param subString To be found
         * @return Offset to found string of NOT_FOUND flag
         */
        uint16 Find(CStaticString subString);

        /**
         * Finds first char in the target
         *
         * @param symbol To be found
         * @return Offset to found symbol of NOT_FOUND flag
         */
        uint16 Find(CHAR symbol);

        /**
         * Get size of string (without termination symbol)
         *
         * @return Current size
         */
        uint16 GetSize();

        /**
         * Get max size of string (its capacity) without termination symbol
         *
         * @return Max capacity (node: this string cannot be expanded)
         */
        uint16 GetCapacity();

        /**
         * Get string special hash value
         *
         * @return Hash
         */
        uint32 GetHash();

        /**
         * Is this symbol in the string
         *
         * @param symbol To be checked
         * @return FOUND or NOT_FOUND flags
         */
        int32 Contains(CHAR symbol);

        /**
         * Type of string (@see StringType)
         *
         * @return Type of this string
         */
        int32 GetType();

        /**
         *
         * Get pointer to standard ASCII string
         *
         * @return CHARS* pointer to buffer
         */
        const CHAR* GetCharsBuffer();

    private:

        uint16 mSize;
        uint16 mCapacity;
        uint32 mHash;
        CHAR*  mBuffer;

    };

} // namespace Berserk

#endif //BERSERKENGINE_CSTATICSTRING_H
