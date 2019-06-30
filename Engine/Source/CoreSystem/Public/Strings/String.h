//
// Created by Egor Orachyov on 25.06.2019.
//

#ifndef BERSERK_STRING_H
#define BERSERK_STRING_H

#include <Strings/StringStatic.h>
#include <Strings/StringDynamic.h>
#include <Strings/StringManager.h>

namespace Berserk
{

    /**
     * [C-style]Name is static string with hardcoded length 64 for
     * temporal task or storing short char keys in hash tables and
     * assets managers (bases)
     */
    typedef StringStatic<char, '\0', 64> Name;

    /**
     * [C-style]Text is static string with hardcoded length 256 for
     * storing big enough texts, labels, or for building another
     * strings from that
     */
    typedef StringStatic<char, '\0', 256> Text;

    /**
     * [C-style]Path is static string which represents OS file system paths
     * for finding and loading and importing resources into the engine
     */
    typedef StringStatic<char, '\0', 256> Path;

    /**
     * [C-style] char character dynamic immutable string in java style with
     * global engine string manager.
     *
     * @node Immutable
     * @note Single-threaded
     */
    typedef StringDynamic String;

} // namespace Berserk

#endif //BERSERK_STRING_H
