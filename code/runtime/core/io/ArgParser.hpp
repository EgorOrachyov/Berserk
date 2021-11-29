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

#ifndef BERSERK_ARGPARSER_HPP
#define BERSERK_ARGPARSER_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/string/String.hpp>

#include <unordered_map>
#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class ArgParser
 * @brief Parser of application input arguments
 */
class BRK_API ArgParser {
public:
    ArgParser() = default;

    /** Add input argument with input value */
    void AddArgument(const String &arg, const String &defaultValue = "");

    /** Parse input */
    void Parse(int count, const char *const *args);

    /** @return True if option specified */
    bool Set(const String &arg) const;

    /** @return True if option specified */
    bool Set(const String &arg, String &value) const;

private:
    std::unordered_map<String, String> mOptions;
    std::unordered_map<String, String> mParsedOptions;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_ARGPARSER_HPP
