//
// Created by Egor Orachyov on 09.06.2018.
//

#ifndef BERSERKENGINE_CSTATICSTRING_H
#define BERSERKENGINE_CSTATICSTRING_H

#include "../Essential/Types.h"
#include "../Essential/Common.h"
#include "../Essential/UsageDescriptors.h"

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
    class DATA_API StaticStringASCII
    {
    public:

        /**
         * String initializing (empty string) by simply passing essential
         * params (could be used without this initialization)
         */
        StaticStringASCII();

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param size Length of string without last terminate symbol
         * @param charsBuffer Pointer to chars buffer to be copied
         */
        StaticStringASCII(uint32 size, const CHAR* charsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param charsBuffer Pointer to chars buffer to be copied
         */
        StaticStringASCII(const CHAR* charsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param anotherString String to be copied
         */
        StaticStringASCII(StaticStringASCII& anotherString);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param size Length of string without last terminate symbol
         * @param charsBuffer Pointer to chars buffer to be copied
         */
        void Init(uint32 size, const CHAR* charsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param charsBuffer Pointer to chars buffer to be copied
         */
        void Init(const CHAR* charsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param anotherString String to be copied
         */
        void Init(StaticStringASCII& anotherString);

        /**
         * Delete string (sets as new empty string)
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
        void Copy(StaticStringASCII& source);

        /**
         * Write mask in target string and insert source in positions marked
         * by symbols %s (works as simple printf)
         *
         * @param source String to placed in %s positions
         * @param mask Writing template
         */
        void Copy(StaticStringASCII& source, StaticStringASCII& mask);

        /**
         * Copy from ASCII string count of characters (while has empty space)
         *
         * @param source Pointer to ASCII string
         * @param count Num of chars to be copied
         */
        void Copy(const CHAR* source, uint32 count);

        /**
         * Copy from ASCII string (while has empty space)
         *
         * @param source Pointer to ASCII string
         */
        void Copy(const CHAR* source);

        /**
         * Writes source in the back of string while it has not used place
         *
         * @param source String to be appended in the back
         */
        void Append(StaticStringASCII& source);

        /**
         * Appends count chars by using ASCII string
         *
         * @param source Pointer to ASCII string
         * @param count Num of chars to be appended
         */
        void Append(const CHAR* source, uint32 count);

        /**
         * Appends chars by using ASCII string
         *
         * @param source Pointer to ASCII string
         */
        void Append(const CHAR* source);

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
        void Insert(StaticStringASCII& source, uint32 offset);

        /**
         * Finds first substring in the target
         *
         * @param subString To be found
         * @return Offset to found string of NOT_FOUND flag
         */
        uint32 Find(StaticStringASCII& subString);

        /**
         * Finds first char in the target
         *
         * @param symbol To be found
         * @return Offset to found symbol of NOT_FOUND flag
         */
        uint32 Find(CHAR symbol);

        /**
         * Get size of string (without termination symbol)
         *
         * @return Current size
         */
        uint32 GetSize();

        /**
         * Get max size of string (its capacity) without termination symbol
         *
         * @return Max capacity (node: this string cannot be expanded)
         */
        uint32 GetCapacity();

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

        uint32 mSize;                       // Current length without '\0' symbol
        uint32 mCapacity;                   // Max available size
        CHAR mBuffer[BUFFER_SIZE_128];      // Buffer

    };

} // namespace Berserk

#endif //BERSERKENGINE_CSTATICSTRING_H
