/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_TSTRING_HPP
#define BERSERK_TSTRING_HPP

#include <BerserkCore/String/TStringUtils.hpp>
#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/TPtrShared.hpp>

namespace Berserk {

    template <typename T, T end>
    class TString {
    public:

        TString() = default;
        TString(const T* str);
        TString(const T* str, uint32 length);
        TString(const TString& str);
        TString(TString&& str) noexcept;

        TString& operator=(const TString& other) = default;
        TString& operator=(TString&& other) noexcept = default;

        bool isEmpty() const { return mData.isNull(); }
        void clear();

        const uint32 getCapacity() const { return isEmpty()? 0: mData->getBufferCapacity() / sizeof(T); }
        const uint32 getLength() const { return isEmpty()? 0: mData->getLength(); }
        const T* getCstr() const { return isEmpty()? getEmptyStr(): mData->getBuffer(); }

    private:

        struct TStringHeader {
            uint32 bufferCapacity = 0;
            uint32 length = 0;

            uint32 getBufferCapacity() const { return bufferCapacity; }
            uint32 getLength() const { return length; }
            T* getBuffer() const { return (T*)((this) + 1); }
        };

        static const T* getEmptyStr();
        static void allocateHeader(TPtrShared<TStringHeader>& header, uint32 stringLength);

        TPtrShared<TStringHeader> mData;
    };

    namespace Details {
        // Handle strings allocations (possibly store in pool)
        void* stringBlockAllocate(uint32 sizeInBytes);
        void stringBlockFree(void* block);
    }

    template <typename T, T end>
    TString<T,end>::TString(const T* str) {

    }

    template <typename T, T end>
    TString<T,end>::TString(const T *str, uint32 length) {

    }

    template <typename T, T end>
    TString<T,end>::TString(const TString<T, end> &str) : mData(str.mData) {

    }

    template <typename T, T end>
    TString<T,end>::TString(TString<T, end> &&str) noexcept : mData(std::move(str.mData)) {

    }

    template <typename T, T end>
    const T* TString<T,end>::getEmptyStr() {
        static const T empty[1] = { end };
        return empty;
    }

    template <typename T, T end>
    void TString<T,end>::allocateHeader(TPtrShared<TString<T, end>::TStringHeader> &header, uint32 stringLength) {
        uint32 sizeOfEnd = sizeof(T);
        uint32 bufferCapacity = stringLength * sizeof(T) + sizeOfEnd;
        uint32 sizeOfHeader = sizeof(TStringHeader);
        uint32 bytesToAlloc = bufferCapacity + sizeOfHeader;

        auto memory = (uint8*) Details::stringBlockAllocate(bytesToAlloc);

        // First ${sizeOfHeader} bytes are header
        // Second ${bufferCapacity} are string buffer

        // create ptr
    }

    using String = TString<char,'\0'>;
    using WString = TString<wchar,L'\0'>;


}

#endif //BERSERK_TSTRING_HPP