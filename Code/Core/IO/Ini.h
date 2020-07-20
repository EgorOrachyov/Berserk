/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_INI_H
#define BERSERK_INI_H

#include <TRef.h>
#include <TPair.h>
#include <Containers/TArray.h>
#include <Platform/File.h>
#include <String/CString.h>
#include <String/CStringBuilder.h>
#include <Reflection/Variant.h>

namespace Berserk {

    /**
     * @brief INI file
     *
     * Allows to access data in the INI file structure.
     * Parses content of the ascii encode file or string.
     *
     * Supports:
     *  - Sections in the file
     *  - Key value params
     *  - Only strings as data
     *
     *  An example of INI file:
     *      ; Default section
     *      [General]
     *      param = value
     *      +param = "value"
     *      ; Yet another section
     *      [Another-Section]
     *      param =
     *      +param = 12321
     *
     */
    class Ini {
    public:

        // How much symbols to preallocate inside string builder
        static const uint32 BUILDER_PREALLOCATE = 128;

        using Value = TPair<CString,CString>;
        using Section = TPair<CString,TArray<Value>>;
        using Content = TArray<Section>;

        Ini(IAlloc &alloc = IAlloc::getSingleton());
        Ini(const char* stream, IAlloc &alloc = IAlloc::getSingleton());
        Ini(const CString& string, IAlloc &alloc = IAlloc::getSingleton());
        Ini(File& file, IAlloc &alloc = IAlloc::getSingleton());
        Ini(const Ini& other) = default;
        Ini(Ini&& other) noexcept = default;

        Ini& operator=(const Ini& other) = default;
        Ini& operator=(Ini&& other) noexcept = default;

        bool isParsed() const { return mIsParsed == EResult::Ok; }

        void getSections(TArray<CString> &sections) const;
        void getValues(const Section& section, const CString& name, TArray<CString> &values) const;
        bool hasSection(const CString& section) const;
        bool hasValue(const Section& section, const CString& param) const;
        TRef<const Section> getSection(const CString& name) const;
        TRef<const CString> getValue(const Section& section, const CString& name) const;

        Content &getContent() { return mContent; }
        const Content &getContent() const { return mContent; }

        CString toString() const;
        void toStingBuilder(CStringBuilder& builder) const;

    private:

        enum class EToken {
            SquareBracketLeft = 1,
            SquareBracketRight = 2,
            String = 3,
            Param = 4,
            Equal = 5,
            Eof = 6,
            Eol = 7
        };

        enum class EResult {
            Ok = 0,
            OkEof = 1,
            UnexpectedSymbol = 2,
            UnexpectedToken = 3
        };

        EResult getToken(const char* stream, uint32 size, EToken& token, uint32& index, uint32& line, uint32& data, bool skipEol = true) const;
        EResult parseSection(const char* stream, uint32 size, Section& section, uint32& index, uint32& line) const;
        EResult parseContent(const char* stream, uint32 size, Content& content) const;

    private:

        Content mContent;
        IAlloc* mAlloc;
        EResult mIsParsed = EResult::Ok;

    };

}

#endif //BERSERK_INI_H