//
// Created by Egor Orachyov on 02.02.2019.
//

#ifndef BERSERK_STATICSTRING_H
#define BERSERK_STATICSTRING_H

#include "StringStream.h"

namespace Berserk
{
    /**
     * [C,W]Name is static string with hardcoded length 64 for
     * temporal task or storing short char keys in hash tables and
     * assets managers (bases)
     *
     * [C,W]Text is static string with hardcoded length 256 for
     * storing big enough texts, labels, or for building another
     * strings from that
     *
     * [C]Path is static string which represents OS file system paths
     * for finding and loading and importing resources into the engine
     */

    typedef StringStream<char,'\0', Buffers::SIZE_64>  CName;
    typedef StringStream<char,'\0', Buffers::SIZE_256> CText;
    typedef StringStream<char,'\0', Buffers::SIZE_512> CPath;

    typedef StringStream<wchar_t , L'\0', Buffers::SIZE_64>  WName;
    typedef StringStream<wchar_t , L'\0', Buffers::SIZE_256> WText;

} // namespace Berserk

#endif //BERSERK_STATICSTRING_H
