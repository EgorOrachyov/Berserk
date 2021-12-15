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
#include <ostream>
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
class StringName {
public:
    /** Construct string id from utf-8 string */
    BRK_API explicit StringName(const String &str = String());

    /** Copy */
    StringName(const StringName &other);

    /** Move */
    StringName(StringName &&other) noexcept;

    /** Safe release id. If it is last id reference, erase it from the cache. */
    BRK_API ~StringName();

    /** Copy */
    BRK_API StringName &operator=(const StringName &other);

    /** Move */
    BRK_API StringName &operator=(StringName &&other) noexcept;

    /** @return True if this ids equal */
    BRK_API bool operator==(const StringName &other) const;

    /** @return True if this ids not equal */
    BRK_API bool operator!=(const StringName &other) const;

    /** @return String of this string name */
    BRK_API const String &GetStr() const;

    /** @return Hash value of this string name */
    BRK_API size_t GetHash() const;

private:
    /** Entry of id in the cache */
    class Node : public RefCnt {
    public:
        explicit Node(String str) : mString(std::move(str)), mHash(std::hash<String>{}(mString)) {}
        const String &GetStr() const { return mString; }
        size_t GetHash() const { return mHash; }

    private:
        String mString;
        size_t mHash;
    };

    void Release();

    /** Entry of id in the cache. Null for empty strings */
    Ref<Node> mNode;

private:
    /** Cache of created string names */
    static std::mutex &GetAccessMutex();
    static std::unordered_map<String, Ref<Node>> &GetCachedNames();
};

/**
 * @}
 */

BRK_NS_END

namespace std {

    template<>
    struct hash<BRK_NS::StringName> {
    public:
        std::size_t operator()(const BRK_NS::StringName &stringName) const {
            return stringName.GetHash();
        }
    };

}// namespace std

inline std::ostream &operator<<(std::ostream &stream, const BRK_NS::StringName &name) {
    stream << name.GetStr();
    return stream;
}

#endif//BERSERK_STRINGNAME_HPP