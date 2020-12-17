/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_STRINGBUILDER_HPP
#define BERSERK_STRINGBUILDER_HPP

#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Containers/Array.hpp>

namespace Berserk {

    class StringBuilder {
    public:
        using CharType = String::CharType;
        using Utils = String::Utils;

        StringBuilder() = default;
        ~StringBuilder() = default;

        void EnsureToAdd(size_t symbols);

        StringBuilder& Add(CharType character);
        StringBuilder& Add(const String& string);
        StringBuilder& Add(const CharType* string);
        StringBuilder& Add(const CharType* string, size_t length);

        uint32 GetLength() const;

        String ToString() const;

    private:
        Array<CharType> mBuffer;

    };

}

#endif //BERSERK_STRINGBUILDER_HPP