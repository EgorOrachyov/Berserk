//
// Created by Egor Orachyov on 03.06.2018.
//

#ifndef BERSERKENGINE_STRINGSFACTORY_H
#define BERSERKENGINE_STRINGSFACTORY_H

#include "CString.h"
#include "WString.h"
#include "StringFlags.h"
#include "CStaticString.h"

namespace Berserk
{

    class StringsFactory
    {
        friend class CString;
        friend class WString;
        friend class CStaticString;

    private:

        typedef struct BlockCHAR
        {
        public:
            int8 mReferenceCount;
            CHAR* mBuffer;
        } BlockCHAR;

        typedef struct BlockWCHAR
        {
        public:
            int32 mReferenceCount;
            WCHAR mBuffer;
        } BlockWCHAR;

    public:

        StringsFactory();
        ~StringsFactory();

        void Init();

        CString CreateASCIIString(StringInitialSize initialSize);

        CString CreateASCIIString(CHAR* buffer, uint16 count);

        CString CreateASCIIString(CHAR* buffer);

        CString CreateASCIIString(CHAR* buffer, StringInitialSize initialSize);

        WString CreateUTF32String(StringInitialSize initialSize);

        WString CreateUTF32String(WCHAR* buffer, uint16 count);

        WString CreateUTF32String(WCHAR* buffer);

        WString CreateUTF32String(WCHAR* buffer, StringInitialSize initialSize);

        CStaticString CreateASCIIStaticString(StringInitialSize initialSize);

        CStaticString CreateASCIIStaticString(CHAR* buffer, uint16 count);

        CStaticString CreateASCIIStaticString(CHAR* buffer);

        CStaticString CreateASCIIStaticString(CHAR* buffer, StringInitialSize initialSize);

    private:

        void FreeBlock(uint16 blockSize);

        CHAR* GetBlockOfCHARs(uint16 blockSize);

        WCHAR* GetBlockOfWCHARs(uint16 blockSize);

    };

    //extern StringsFactory globalStringFactory;

} // namespace Berserk

#endif //BERSERKENGINE_STRINGSFACTORY_H
