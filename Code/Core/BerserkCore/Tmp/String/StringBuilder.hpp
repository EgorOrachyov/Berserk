/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_STRINGBUILDER_HPP
#define BERSERK_STRINGBUILDER_HPP

#include <BerserkCore/Containers/TArray.hpp>
#include <BerserkCore/String/String.hpp>

namespace Berserk {

    /**
     * @brief String Builder
     * Basic class for fast strings constructing.
     * @none Uses generic head-based allocator
     */
    class StringBuilder {
    public:
        using CHAR = String::CHAR;

        StringBuilder() = default;
        ~StringBuilder() = default;

        StringBuilder& operator=(const StringBuilder& other) = default;
        StringBuilder& operator=(StringBuilder&& other) noexcept = default;

        // Construction functions
        StringBuilder& Append(CHAR symbol);
        StringBuilder& Append(const CHAR* string);
        StringBuilder& Append(const CHAR* string, uint32 length);
        StringBuilder& Append(const String& string);
        StringBuilder& Append(uint32 value);
        StringBuilder& Append(uint64 value);
        StringBuilder& Append(int32 value);
        StringBuilder& Append(float value, uint32 precision = 5);
        StringBuilder& Append(double value, uint32 precision = 5);

        // Syntax sugar section
        StringBuilder& operator<<(StringBuilder::CHAR symbol);
        StringBuilder& operator<<(const StringBuilder::CHAR* string);
        StringBuilder& operator<<(const String& string);
        StringBuilder& operator<<(uint32 value);
        StringBuilder& operator<<(uint64 value);
        StringBuilder& operator<<(int32 value);
        StringBuilder& operator<<(float value);
        StringBuilder& operator<<(double value);

        /** @return String representation of the builder content */
        String ToString() const;
        uint32 GetBufferSize() const { return mBuffer.GetSize(); }
        const TArray<char>& GetBuffer() const { return mBuffer; }

    private:
        TArray<char> mBuffer;
    };

}

#endif //BERSERK_STRINGBUILDER_HPP