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

#include <core/string/StringName.hpp>

BRK_NS_BEGIN

StringName::StringName(const String &str) {
    if (!str.empty()) {
        std::lock_guard<std::mutex> guard(GetAccessMutex());

        auto &cachedNames = GetCachedNames();
        auto query = cachedNames.find(str);

        if (query != cachedNames.end()) {
            mNode = query->second;
            return;
        }

        mNode = Ref<Node>(new Node(str));
        cachedNames.emplace(str, mNode);
    }
}

StringName::~StringName() {
    if (mNode.IsNotNull() && mNode.IsUnique()) {
        std::lock_guard<std::mutex> guard(GetAccessMutex());
        GetCachedNames().erase(mNode->GetStr());
    }
}

bool StringName::operator==(const StringName &other) const {
    return mNode == other.mNode;
}

bool StringName::operator!=(const StringName &other) const {
    return mNode != other.mNode;
}

const String &StringName::GetStr() const {
    static String empty;
    return mNode.IsNotNull() ? mNode->GetStr() : empty;
}

size_t StringName::GetHash() const {
    return mNode.IsNotNull() ? mNode->GetHash() : std::hash<String>{}(GetStr());
}

std::mutex &StringName::GetAccessMutex() {
    static std::mutex accessMutex;
    return accessMutex;
}

std::unordered_map<String, Ref<StringName::Node>> &StringName::GetCachedNames() {
    static std::unordered_map<String, Ref<StringName::Node>> cachedNames;
    return cachedNames;
}

BRK_NS_END
