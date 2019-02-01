//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_STRINGSINCLUDE_H
#define BERSERK_STRINGSINCLUDE_H

#include "Public/Strings/StringStream.h"

namespace Berserk
{

    typedef StringStream<char,'\0',Buffers::SIZE_64> CName;

    typedef StringStream<wchar_t , L'\0', Buffers::SIZE_64> WName;

} // namespace Berserk

#endif //BERSERK_STRINGSINCLUDE_H
