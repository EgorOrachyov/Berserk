//
// Created by Egor Orachyov on 03.06.2018.
//

#ifndef BERSERKENGINE_STRINGSFACTORY_H
#define BERSERKENGINE_STRINGSFACTORY_H

#include "StringASCII.h"
#include "StringUTF32.h"
#include "StringFlags.h"
#include "StaticStringASCII.h"

namespace Berserk
{

    class StringsFactory
    {
        friend class StringASCII;
        friend class StringUTF32;
        friend class StaticStringASCII;

    private:

        /*

        typedef struct BlockCHAR
        {
        public:
            int64 mReferenceCount;
            CHAR* mBuffer;
        } BlockCHAR;

        typedef struct BlockWCHAR
        {
        public:
            int64 mReferenceCount;
            WCHAR* mBuffer;
        } BlockWCHAR;

        */

    public:

        StringsFactory();
        ~StringsFactory();

        void Init();

        StringASCII* CreateASCIIString(StringInitialSize initialSize);

        StringASCII* CreateASCIIString(CHAR* buffer, uint16 count);

        StringASCII* CreateASCIIString(CHAR* buffer);

        StringASCII* CreateASCIIString(CHAR* buffer, StringInitialSize initialSize);

        StringUTF32* CreateUTF32String(StringInitialSize initialSize);

        StringUTF32* CreateUTF32String(WCHAR* buffer, uint16 count);

        StringUTF32* CreateUTF32String(WCHAR* buffer);

        StringUTF32* CreateUTF32String(WCHAR* buffer, StringInitialSize initialSize);

        StaticStringASCII* CreateASCIIStaticString(StringInitialSize initialSize);

        StaticStringASCII* CreateASCIIStaticString(CHAR* buffer, uint16 count);

        StaticStringASCII* CreateASCIIStaticString(CHAR* buffer);

        StaticStringASCII* CreateASCIIStaticString(CHAR* buffer, StringInitialSize initialSize);

    private:

        CHAR* GetBlockOfCHARs(uint16 blockSize, CHAR* oldBlock);

        WCHAR* GetBlockOfWCHARs(uint16 blockSize, WCHAR* oldBlock);

    };

    //extern StringsFactory globalStringFactory;

} // namespace Berserk

#endif //BERSERKENGINE_STRINGSFACTORY_H
