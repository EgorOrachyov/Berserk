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
    class DATA_API StaticStringUTF32
    {
    public:

        /**
         * String initializing (empty string) by simply passing essential
         * params (could be used without this initialization)
         */
        StaticStringUTF32();

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param size Length of string without last terminate symbol
         * @param wcharsBuffer Pointer to wchars buffer to be copied
         */
        StaticStringUTF32(uint32 size, const WCHAR* wcharsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param wcharsBuffer Pointer to wchars buffer to be copied
         */
        StaticStringUTF32(const WCHAR* wcharsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param anotherString String to be copied
         */
        StaticStringUTF32(StaticStringUTF32& anotherString);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param size Length of string without last terminate symbol
         * @param wcharsBuffer Pointer to wchars buffer to be copied
         */
        void Init(uint32 size, const WCHAR* wcharsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param wcharsBuffer Pointer to wchars buffer to be copied
         */
        void Init(const WCHAR* wcharsBuffer);

        /**
         * String initializing by simply passing essential
         * params (could be used without this initialization)
         *
         * @param anotherString String to be copied
         */
        void Init(StaticStringUTF32& anotherString);

        /**
         * Delete string (sets as new empty string)
         */
        void Delete();

        /**
         * Set empty string with 0 length
         */
        void Empty();

        /**
         * By wchar coping of source string
         *
         * @param source String to be copied
         */
        void Copy(StaticStringUTF32& source);

        /**
         * Write mask in target string and insert source in positions marked
         * by symbols %s (works as simple printf)
         *
         * @param source String to placed in %s positions
         * @param mask Writing template
         */
        void Copy(StaticStringUTF32& source, StaticStringUTF32& mask);

        /**
         * Copy from UT32 string count of wcharacters (while has empty space)
         *
         * @param source Pointer to UT32 string
         * @param count Num of wchars to be copied
         */
        void Copy(const WCHAR* source, uint32 count);

        /**
         * Copy from UT32 string (while has empty space)
         *
         * @param source Pointer to UT32 string
         */
        void Copy(const WCHAR* source);

        /**
         * Writes source in the back of string while it has not used place
         *
         * @param source String to be appended in the back
         */
        void Append(StaticStringUTF32& source);

        /**
         * Appends count wchars by using UT32 string
         *
         * @param source Pointer to UT32 string
         * @param count Num of wchars to be appended
         */
        void Append(const WCHAR* source, uint32 count);

        /**
         * Appends wchars by using UT32 string
         *
         * @param source Pointer to UT32 string
         */
        void Append(const WCHAR* source);

        /**
         * Append UT32 symbol in the end of string
         *
         * @param symbol To be added
         */
        void Append(WCHAR symbol);

        /**
         * Inserts substring source from the offset position
         *
         * @param source To be inserted
         * @param offset Start index of insertion (0 - insert from beginning, target length -
         *        insert from the end)
         */
        void Insert(StaticStringUTF32& source, uint32 offset);

        /**
         * Finds first substring in the target
         *
         * @param subString To be found
         * @return Offset to found string of NOT_FOUND flag
         */
        uint32 Find(StaticStringUTF32& subString);

        /**
         * Finds first wchar in the target
         *
         * @param symbol To be found
         * @return Offset to found symbol of NOT_FOUND flag
         */
        uint32 Find(WCHAR symbol);

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
        int32 Contains(WCHAR symbol);

        /**
         * Type of string (@see StringType)
         *
         * @return Type of this string
         */
        int32 GetType();

        /**
         *
         * Get pointer to standard UT32 string
         *
         * @return WCHARS* pointer to buffer
         */
        const WCHAR* GetCharsBuffer();

        static StaticStringUTF32 Get(const WCHAR* buffer)
        {
            StaticStringUTF32 tmp;
            tmp.Copy(buffer);
            return tmp;
        }

    private:

        uint32 mSize;                       // Current length without L'\0' symbol
        uint32 mCapacity;                   // Max available size
        WCHAR mBuffer[BUFFER_SIZE_128];     // Buffer

    };

} // namespace Berserk

#endif //BERSERKENGINE_STATICSTRINGUTF32_H
