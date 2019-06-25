//
// Created by Egor Orachyov on 17.03.2019.
//

#ifndef BERSERK_STRING_H
#define BERSERK_STRING_H

#include "StaticString.h"
#include "DynamicString.h"

namespace Berserk
{

    /**
     * [C,W]String is dynamic wide and short character string with
     * allocating its data in global StringPool. Allows to create strings
     * with total char buffer less than 1024 bytes
     *
     * Note: 1024 chars for CString
     * Note: 256 wide chars for WString
     */

    typedef DynamicString<char, '\0'> CString;
    typedef DynamicString<wchar_t , L'\0'> WString;

}

#endif //BERSERK_STRING_H
