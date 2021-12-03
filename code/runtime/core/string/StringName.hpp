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

#ifndef BERSERK_STRINGNAME_HPP
#define BERSERK_STRINGNAME_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/string/String.hpp>
#include <core/templates/Ref.hpp>
#include <core/templates/RefCnt.hpp>

#include <mutex>
#include <unordered_map>
#include <utility>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class StringName
 * @brief Cached shared utf-8 string id
 *
 * Immutable reference counted shared utf-8 encoded string object.
 * Must be used for string named objects, handlers and ids.
 *
 * This string object must be used in case, when you need
 * lots of references to the single immutable string identifier.
 *
 * Supports fast `O(1)` string equality/inequality checks.
 */
class BRK_API StringName {
public:
    /** Construct string id from utf-8 string */
    explicit StringName(const String &str);

    /** Safe release id. If it is last id reference, erase it from the cache. */
    ~StringName();

    /** @return True if this ids equal */
    bool operator==(const StringName &other) const;

    /** @return True if this ids not equal */
    bool operator!=(const StringName &other) const;

    /** @return String of this string name */
    const String &GetStr() const;

private:
    /** Entry of id in the cache */
    class Node : public RefCnt {
    public:
        explicit Node(String str) : mString(std::move(str)) {}
        const String &GetStr() const { return mString; }

    private:
        String mString;
    };

    /** Entry of id in the cache. Null for empty strings */
    Ref<Node> mNode;

    /** Cache of created string names */
    static std::mutex mAccessMutex;
    static std::unordered_map<String, Ref<Node>> mCachedNames;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_STRINGNAME_HPP