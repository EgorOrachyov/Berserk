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

#include <BerserkCore/Templates/Singleton.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Templates/HashTable.hpp>
#include <BerserkCore/Platform/Synchronization.hpp>

namespace Berserk {

    class StringNameTable {
    public:

        /** @return Total number of unique string entries in the table */
        static uint64 GetEntriesCount() {
            return Impl::Instance().GetEntriesCount();
        }

        /** Dump string enters, stored in the table*/
        static void GetEntries(Array<String> &array) {
            Impl::Instance().GetEntries(array);
        }

    protected:
        friend class StringName;
        class Impl;

        class Entry {
        public:
            explicit Entry(String string) : mData(std::move(string)) {}
            ~Entry() = default;

            const String& GetString() const { return mData; }

        private:
            friend class Impl;

            void AddRefs() { mRefs +=1 ; }
            uint64 Release() { auto r = mRefs; mRefs -= 1; return r; }

        private:
            String mData;
            uint64 mRefs = 0;
        };

        class Impl: public Singleton<Impl> {
        public:
            Impl();
            ~Impl();

            Entry* GetOrCreateEntry(const String& name);
            void AddRefs(Entry* entry);
            void Release(Entry* entry);
            uint64 GetEntriesCount() const;
            void GetEntries(Array<String> &array);

        private:
            HashTable<String, Entry> mNameMap;
            mutable SpinMutex mMutex;
        };
    };

    /**
     * @brief String Name
     *
     * Immutable reference counted shared utf-8 encoded string object.
     * Must be used for string named objects, handlers and ids.
     *
     * This string object must be used in case, when you need
     * lots of references to the single immutable string identifier.
     */
    class StringName {
    public:
        using Utils = String::Utils;
        using CharType = String::CharType;

        StringName() = default;
        StringName(const CharType* string);
        StringName(const CharType* string, uint32 length);
        StringName(const String& string);
        StringName(const StringName& string);
        StringName(StringName&& string) noexcept;

        ~StringName();

        StringName& operator=(const StringName& other);
        StringName& operator=(StringName&& other) noexcept;

        /** @return lexicographical '==' */
        bool operator==(const StringName &other) const;

        /** @return lexicographical '!=' */
        bool operator!=(const StringName &other) const;

        /** @return lexicographical '<=' */
        bool operator<=(const StringName &other) const;

        /** @return lexicographical '>=' */
        bool operator>=(const StringName &other) const;

        /** @return lexicographical '<' */
        bool operator<(const StringName &other) const;

        /** @return lexicographical '>' */
        bool operator>(const StringName &other) const;

        /** @return String length in CharType units */
        uint32 GetLength() const;

        /** @return C compatible null-terminated string buffer */
        const CharType* GetStr_C() const;

        /** @return This entry string object ref */
        const String& GetStr() const;

        /** @return Hash of the string content */
        uint32 Hash() const;

        /** @return Concatenated c-style string and other string */
        friend String operator+(const CharType* left, const StringName& right) { return String(left) + right.GetStr(); }

        /** @return Concatenated c-style string and other string */
        friend String operator+(const StringName& left, const String& right) { return left.GetStr() + right; }

    private:
        bool IsNull() const { return mEntry == nullptr; }
        bool IsNotNull() const { return mEntry != nullptr; }
        void FromString(const String& string);

        using Entry = StringNameTable::Entry;

        Entry* mEntry = nullptr;
    };

    /**
     * Implement contracts for the string name
     */
    template<>
    class Equals<StringName> {
    public:
        bool operator()(const StringName &a, const StringName &b) const {
            return a == b;
        }
    };

    template<>
    class Hash<StringName> {
    public:
        uint32 operator()(const StringName &a) const {
            return a.Hash();
        }
    };

    template<>
    class CompareLess<StringName> {
    public:
        bool operator()(const String &a, const String &b) const {
            return a < b;
        }
    };

    template <>
    class TextPrint<StringName> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, const StringName& string) const {
            stream.Add(string.GetStr_C());
        }
    };
}

#endif //BERSERK_STRINGNAME_HPP