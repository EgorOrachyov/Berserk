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

    /**
     * Fixed size tmp string [dynamic string wrapper] for simple and
     * fast hash map look up
     *
     * @warning Must not be copied or assigned to another string
     */
    typedef StringDynamic_Wrapper Wrapper;

    /**
     * Convert int value to string
     * @param value To convert
     * @return String representation of the value
     */
    StringDynamic toString(int32 value);

    /**
     * Convert float value to string
     * @param value To convert
     * @return String representation of the value
     */
    StringDynamic toString(float32 value);

    /**
     * Convert to string
     * @param value To convert
     * @return String representation of vector
     */
    StringDynamic toString(const Vec2f& v);

    /**
     * Convert to string
     * @param value To convert
     * @return String representation of vector
     */
    StringDynamic toString(const Vec3f& v);

    /**
     * Convert to string
     * @param value To convert
     * @return String representation of vector
     */
    StringDynamic toString(const Vec4f& v);

    /**
     * Convert to string
     * @param value To convert
     * @return String representation of vector
     */
    StringDynamic toString(const Quatf& q);

    /**
     * Convert to string
     * @param degrees Angle to convert
     * @return String representation of the angle
     */
    StringDynamic toString(const Degrees& degrees);

    /**
     * Convert to string
     * @param radians Angle to convert
     * @return String representation of the angle
     */
    StringDynamic toString(const Radians& radians);

} // namespace Berserk

#endif //BERSERK_STRING_H
