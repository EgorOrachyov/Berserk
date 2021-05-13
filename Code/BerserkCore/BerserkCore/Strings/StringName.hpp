/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_STRINGNAME_HPP
#define BERSERK_STRINGNAME_HPP

#include <BerserkCore/Misc/Singleton.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Containers/Map.hpp>
#include <BerserkCore/Platform/Synchronization.hpp>

namespace Berserk {

    class StringNameTable {
    public:

        /** @return Total number of unique string entries in the table */
        static size_t GetEntriesCount() {
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
            size_t GetEntriesCount() const;
            void GetEntries(Array<String> &array);

        private:
            Map<String, Entry> mNameMap;
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