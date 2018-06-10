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
        void Init(uint16 size, uint16 capacity, uint32 hash, int8* charsBuffer);

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
         * Writes source in the back of string while it has not used place
         *
         * @param source String to be appended in the back
         */
        void Append(CStaticString source);

        /**
         *
         * @param symbol
         */
        void Append(int8 symbol);

        /**
         *
         * @param source
         * @param offset
         */
        void Insert(CStaticString source, uint16 offset);

        /**
         *
         * @param subString
         * @return
         */
        uint16 Find(CStaticString subString);

        /**
         *
         * @param symbol
         * @return
         */
        uint16 Find(int8 symbol);

        /**
         *
         * @return
         */
        uint16 GetSize();

        /**
         *
         * @return
         */
        uint16 GetCapacity();

        /**
         *
         * @return
         */
        uint32 GetHash();

        /**
         *
         * @param symbol
         * @return
         */
        int32 Contains(int8 symbol);

        /**
         *
         * @return
         */
        const int8* GetCharsBuffer();

    private:

        uint16 mSize;
        uint16 mCapacity;
        uint32 mHash;
        int8*  mBuffer;

    };

} // namespace Berserk

#endif //BERSERKENGINE_CSTATICSTRING_H
