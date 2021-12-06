/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_OUTPUT_HPP
#define BERSERK_OUTPUT_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/string/String.hpp>

BRK_NS_BEGIN

/**
 * @class Output
 * @brief Unified console output for utf-8 text
 */
class Output final {
public:
    BRK_API Output() = default;
    BRK_API ~Output() = default;

    /**
     * @brief Write text to system console
     *
     * Writes text to system console.
     * Automatically converts encoding of th string.
     *
     * @param text Utf-8 encoded string text
     */
    BRK_API void Write(const String &text);

    /**
     * @brief Write warning text to system console
     *
     * Writes text to system console.
     * Automatically converts encoding of th string.
     *
     * @param text Utf-8 encoded string text
     */
    BRK_API void WriteWarning(const String &text);

    /**
     * @brief Write error text to system console
     *
     * Writes text to system console.
     * Automatically converts encoding of th string.
     *
     * @param text Utf-8 encoded string text
     */
    BRK_API void WriteError(const String &text);
};

BRK_NS_END

#endif//BERSERK_OUTPUT_HPP
