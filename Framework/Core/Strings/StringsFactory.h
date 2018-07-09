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

        CString* CreateASCIIString(StringInitialSize initialSize);

        CString* CreateASCIIString(CHAR* buffer, UINT16 count);

        CString* CreateASCIIString(CHAR* buffer);

        CString* CreateASCIIString(CHAR* buffer, StringInitialSize initialSize);

        WString* CreateUTF32String(StringInitialSize initialSize);

        WString* CreateUTF32String(WCHAR* buffer, UINT16 count);

        WString* CreateUTF32String(WCHAR* buffer);

        WString* CreateUTF32String(WCHAR* buffer, StringInitialSize initialSize);

        CStaticString* CreateASCIIStaticString(StringInitialSize initialSize);

        CStaticString* CreateASCIIStaticString(CHAR* buffer, UINT16 count);

        CStaticString* CreateASCIIStaticString(CHAR* buffer);

        CStaticString* CreateASCIIStaticString(CHAR* buffer, StringInitialSize initialSize);

    private:

        CHAR* GetBlockOfCHARs(UINT16 blockSize, CHAR* oldBlock);

        WCHAR* GetBlockOfWCHARs(UINT16 blockSize, WCHAR* oldBlock);

    };

    //extern StringsFactory globalStringFactory;

} // namespace Berserk

#endif //BERSERKENGINE_STRINGSFACTORY_H
