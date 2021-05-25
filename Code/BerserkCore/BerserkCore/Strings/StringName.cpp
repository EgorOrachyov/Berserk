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

#include <BerserkCore/Strings/StringName.hpp>

namespace Berserk {

    StringNameTable::Impl::Impl() {
        Provide(this);
    }

    StringNameTable::Impl::~Impl() {
        assert(mNameMap.IsEmpty());
        Remove(this);
    }

    StringNameTable::Entry* StringNameTable::Impl::GetOrCreateEntry(const String &name) {
        Guard<SpinMutex> guard(mMutex);

        Entry* entry = mNameMap.GetPtr(name);

        if (!entry) {
            Entry newEntry(name);
            entry = &mNameMap.Emplace(name, std::move(newEntry));
        }

        entry->AddRefs();

        return entry;
    }

    void StringNameTable::Impl::AddRefs(StringNameTable::Entry *entry) {
        assert(entry);

        Guard<SpinMutex> guard(mMutex);
        entry->AddRefs();
    }

    void StringNameTable::Impl::Release(StringNameTable::Entry *entry) {
        assert(entry);

        Guard<SpinMutex> guard(mMutex);
        auto refs = entry->Release();

        // We were last
        if (refs == 1) {
            mNameMap.Remove(entry->GetString());
        }
    }

    size_t StringNameTable::Impl::GetEntriesCount() const {
        Guard<SpinMutex> guard(mMutex);
        return mNameMap.GetSize();
    }

    void StringNameTable::Impl::GetEntries(Array<String> &array) {
        Guard<SpinMutex> guard(mMutex);

        array.EnsureToAdd(mNameMap.GetSize());
        for (const auto& p: mNameMap) {
            array.Add(p.GetSecond().GetString());
        }
    }

    StringName::StringName(const CharType *string) {
        FromString(String(string));
    }

    StringName::StringName(const CharType *string, uint32 length) {
        FromString(String(string, length));
    }

    StringName::StringName(const String &string) {
        FromString(string);
    }

    StringName::StringName(const StringName &string) {
        mEntry = string.mEntry;

        if (mEntry) {
            auto& instance = StringNameTable::Impl::Instance();
            instance.AddRefs(mEntry);
        }
    }

    StringName::StringName(StringName &&string) noexcept {
        mEntry = string.mEntry;
        string.mEntry = nullptr;
    }

    StringName::~StringName() {
        if (mEntry) {
            auto& instance = StringNameTable::Impl::Instance();
            instance.Release(mEntry);
            mEntry = nullptr;
        }
    }

    StringName& StringName::operator=(const StringName& other) {
        if (this == &other) {
            return *this;
        }

        this->~StringName();
        new (this) StringName(other);
        return *this;
    }

    StringName& StringName::operator=(StringName&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        this->~StringName();
        new (this) StringName(std::move(other));
        return *this;
    }

    bool StringName::operator==(const StringName &other) const {
        return mEntry == other.mEntry;
    }

    bool StringName::operator!=(const StringName &other) const {
        return mEntry != other.mEntry;
    }

    bool StringName::operator<=(const StringName &other) const {
        return Utils::Compare(GetStr_C(), other.GetStr_C()) <= 0;
    }

    bool StringName::operator>=(const StringName &other) const {
        return Utils::Compare(GetStr_C(), other.GetStr_C()) >= 0;
    }

    bool StringName::operator<(const StringName &other) const {
        return Utils::Compare(GetStr_C(), other.GetStr_C()) < 0;
    }

    bool StringName::operator>(const StringName &other) const {
        return Utils::Compare(GetStr_C(), other.GetStr_C()) > 0;
    }

    uint32 StringName::GetLength() const {
        return IsNotNull()? mEntry->GetString().GetLength(): 0;
    }

    const StringName::CharType* StringName::GetStr_C() const {
        static const char empty[] = BERSERK_TEXT("");
        return IsNotNull() ? mEntry->GetString().GetStr_C() : empty;
    }

    const String & StringName::GetStr() const {
        static String empty;
        return IsNotNull()? mEntry->GetString(): empty;
    }

    uint32 StringName::Hash() const {
        return Crc32::Hash(GetStr_C(), GetLength());
    }

    void StringName::FromString(const String &string) {
        auto& instance = StringNameTable::Impl::Instance();
        mEntry = instance.GetOrCreateEntry(string);
    }

}
