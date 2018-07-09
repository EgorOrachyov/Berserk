//
// Created by Egor Orachyov on 03.06.2018.
//

#ifndef BERSERKENGINE_STRINGSFACTORY_H
#define BERSERKENGINE_STRINGSFACTORY_H

#include "CHARString.h"
#include "WCAHRString.h"
#include "StringFlags.h"
#include "CHARStaticString.h"

namespace Berserk
{

    class StringsFactory
    {
        friend class CHARString;
        friend class WCAHRString;
        friend class CHARStaticString;

    private:

        /*

        typedef struct BlockCHAR
        {
        public:
            INT64 mReferenceCount;
            CHAR* mBuffer;
        } BlockCHAR;

        typedef struct BlockWCHAR
        {
        public:
            INT64 mReferenceCount;
            WCHAR* mBuffer;
        } BlockWCHAR;

        */

    public:

        StringsFactory();
        ~StringsFactory();

        void Init();

        CHARString* CreateASCIIString(StringInitialSize initialSize);

        CHARString* CreateASCIIString(CHAR* buffer, UINT16 count);

        CHARString* CreateASCIIString(CHAR* buffer);

        CHARString* CreateASCIIString(CHAR* buffer, StringInitialSize initialSize);

        WCAHRString* CreateUTF32String(StringInitialSize initialSize);

        WCAHRString* CreateUTF32String(WCHAR* buffer, UINT16 count);

        WCAHRString* CreateUTF32String(WCHAR* buffer);

        WCAHRString* CreateUTF32String(WCHAR* buffer, StringInitialSize initialSize);

        CHARStaticString* CreateASCIIStaticString(StringInitialSize initialSize);

        CHARStaticString* CreateASCIIStaticString(CHAR* buffer, UINT16 count);

        CHARStaticString* CreateASCIIStaticString(CHAR* buffer);

        CHARStaticString* CreateASCIIStaticString(CHAR* buffer, StringInitialSize initialSize);

    private:

        CHAR* GetBlockOfCHARs(UINT16 blockSize, CHAR* oldBlock);

        WCHAR* GetBlockOfWCHARs(UINT16 blockSize, WCHAR* oldBlock);

    };

    //extern StringsFactory globalStringFactory;

} // namespace Berserk

#endif //BERSERKENGINE_STRINGSFACTORY_H
