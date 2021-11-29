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

#include <core/io/ArgumentParser.hpp>

#include <cassert>

BRK_NS_BEGIN

void ArgumentParser::AddArgument(const String &arg, const String &defaultValue) {
    mOptions[arg] = defaultValue;
}

void ArgumentParser::Parse(int count, const char *const *args) {
    String::size_type pos;

    for (int i = 0; i < count; i++) {
        String arg(args[i]);

        if ((pos = arg.find('=')) != String::npos)
            mParsedOptions.emplace(arg.substr(0, pos), arg.substr(pos + 1));
        else
            mParsedOptions.emplace(arg, "");
    }
}

bool ArgumentParser::Set(const String &arg) const {
    return mParsedOptions.find(arg) != mParsedOptions.end();
}

bool ArgumentParser::Set(const String &arg, String &value) const {
    auto query = mParsedOptions.find(arg);
    auto found = query != mParsedOptions.end();

    if (found)
        value = query->second;
    else {
        auto def = mOptions.find(arg);
        assert(def != mOptions.end());
        value = def->second;
    }

    return found;
}

BRK_NS_END