/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/Ini.h>
#include <ErrorMacro.h>

#define EXPECT_EOF() { if (token == EToken::Eof) return EResult::OkEof; }
#define FETCH_NEXT() { result = getToken(stream, size, token, index, line, data); if (result != EResult::Ok ) return result; }
#define FETCH_NEXT_EOL() { result = getToken(stream, size, token, index, line, data, false); if (result != EResult::Ok ) return result; }
#define EXPECT_TOKEN(tk) { if (EToken:: tk != token) return EResult::UnexpectedToken; }
#define EXPECT_TOKEN2(tk1,tk2) { if (EToken:: tk1 != token && EToken:: tk2 != token) return EResult::UnexpectedToken; }


namespace Berserk {

    bool isAlphabetSymbol(char symbol) {
        return ('0' <= symbol && symbol <= '9') ||
               ('a' <= symbol && symbol <= 'z') ||
               ('A' <= symbol && symbol <= 'Z') ||
               (symbol == '_') ||
               (symbol == '.') ||
               (symbol == '+') ||
               (symbol == '-');
    }

    Ini::Ini(IAlloc &alloc) : mAlloc(&alloc), mContent(alloc) {

    }

    Ini::Ini(const char *stream, IAlloc &alloc) : Ini(alloc) {
        auto size = CStringUtility::length(stream);
        mIsParsed = parseContent(stream, size, mContent);
    }

    Ini::Ini(const CString &string, IAlloc &alloc) : Ini(alloc) {
        auto stream = string.data();
        auto size = string.length();
        mIsParsed = parseContent(stream, size, mContent);
    }

    Ini::Ini(File &file, IAlloc &alloc) : Ini(alloc) {
        BERSERK_COND_ERROR_RET(file.isOpen(), "File must be open");

        TArray<char> data;
        auto size = file.getSize();
        data.resize(size);
        file.read(data.data(), size);
        mIsParsed = parseContent(data.data(), size, mContent);
    }

    void Ini::getSections(TArray<CString> &sections) const {
        sections.ensureToAdd(mContent.size());

        for (auto& section: mContent) {
            sections.add(section.first());
        }
    }
    
    void Ini::getValues(const Ini::Section &section, const CString &name, TArray<CString> &values) const {
        for (auto& value: section.second()) {
            if (value.first() == name)
                values.emplace(value.second());
        }
    }

    bool Ini::hasSection(const CString &section) const {
        return getSection(section).isNotNull();
    }

    bool Ini::hasValue(const Ini::Section &section, const CString &param) const {
        return getValue(section, param).isNotNull();
    }

    TRef<const Ini::Section> Ini::getSection(const CString &name) const {
        for (auto& section: mContent) {
            if (section.first() == name)
                return &section;
        }

        return nullptr;
    }

    TRef<const CString> Ini::getValue(const Ini::Section &section, const CString &name) const {
        for (auto& value: section.second()) {
            if (value.first() == name)
                return &value.second();
        }

        return nullptr;
    }

    CString Ini::toString() const {
        CStringBuilder builder;
        toStingBuilder(builder);
        return builder.toString();
    }

    void Ini::toStingBuilder(CStringBuilder &builder) const {
        builder.ensureCapacity(BUILDER_PREALLOCATE);

        for (auto& section: mContent) {
            auto& name = section.first();

            builder.append('[');
            builder.append(name);
            builder.append("]\n");

            for (auto& value: section.second()) {
                builder.append(value.first());
                builder.append(" = ");
                builder.append(value.second());
                builder.append('\n');
            }
        }
    }

    Ini::EResult Ini::getToken(const char *stream, uint32 size, Ini::EToken &token, uint32 &index, uint32 &line, uint32 &data, bool skipEol) const {
        while (true) {
            if (index >= size) {
                token = EToken::Eof;
                return EResult::Ok;
            }

            auto symbol = stream[index];

            switch (symbol) {
                case '\t':
                    index += 1;
                    break;
                case ' ':
                    index += 1;
                    break;
                case ';':
                    while (index < size && stream[index] != '\n')
                        index += 1;
                    break;
                case '\n':
                    line += 1;
                    index += 1;
                    if (!skipEol) {
                        token = EToken::Eol;
                        return EResult::Ok;
                    }
                    break;
                case '[':
                    token = EToken::SquareBracketLeft;
                    index += 1;
                    return EResult::Ok;
                case ']':
                    token = EToken::SquareBracketRight;
                    index += 1;
                    return EResult::Ok;
                case '=':
                    token = EToken::Equal;
                    index += 1;
                    return EResult::Ok;
                case '\0':
                    token = EToken::Eof;
                    return EResult::Ok;
                case '\"': {
                    uint32 i = index + 1;
                    while (i < size && stream[i] != '\"') {
                        i += 1;
                    }
                    if (i < size) {
                        token = EToken::String;
                        data = index + 1;
                        index = i + 1;
                        return EResult::Ok;
                    }
                    return EResult::UnexpectedSymbol;
                }
                default:
                    if (isAlphabetSymbol(symbol)) {
                        uint32 i = index + 1;
                        while (i < size && isAlphabetSymbol(stream[i])) {
                            i += 1;
                        }
                        token = EToken::Param;
                        data = index;
                        index = i;
                        return EResult::Ok;
                    }
                    return EResult::UnexpectedSymbol;
            }
        }
    }

    Ini::EResult Ini::parseSection(const char *stream, uint32 size, Ini::Section &section, uint32 &index, uint32 &line) const {
        EResult result;
        EToken token;
        uint32 data;

        while (true) {
            FETCH_NEXT();
            EXPECT_EOF();

            if (token == EToken::SquareBracketLeft)
                return EResult::Ok;

            EXPECT_TOKEN(Param);

            uint32 valueNameBegin = data;
            uint32 valueNameLength = index - data;

            FETCH_NEXT();
            EXPECT_TOKEN(Equal);

            FETCH_NEXT_EOL();
            EXPECT_EOF();

            if (token == EToken::Eol) {
                CString valueName(&stream[valueNameBegin], valueNameLength);
                CString valueData;
                section.second().emplace(std::move(valueName), std::move(valueData));
            }
            else {
                EXPECT_TOKEN2(Param,String);

                uint32 valueDataBegin = data;
                uint32 valueDataLength = index - data - (token == EToken::String? 1: 0);

                CString valueName(&stream[valueNameBegin], valueNameLength);
                CString valueData(&stream[valueDataBegin], valueDataLength);

                section.second().emplace(std::move(valueName), std::move(valueData));

                FETCH_NEXT_EOL();
                EXPECT_EOF();
                EXPECT_TOKEN(Eol);
            }
        }
    }

    Ini::EResult Ini::parseContent(const char *stream, uint32 size, Ini::Content &content) const {
        EResult result;
        EToken token;
        uint32 line = 0;
        uint32 data;
        uint32 index = 0;

        FETCH_NEXT();
        EXPECT_EOF();
        EXPECT_TOKEN(SquareBracketLeft);

        while (true) {
            FETCH_NEXT();
            EXPECT_TOKEN(Param);

            uint32 sectionNameBegin = data;
            uint32 sectionNameLength = index - data;

            FETCH_NEXT();
            EXPECT_TOKEN(SquareBracketRight);

            FETCH_NEXT_EOL();

            bool found = true;
            Section* section;
            Section newSection;
            CString sectionName(&stream[sectionNameBegin], sectionNameLength);

            section = (Section*) getSection(sectionName).getPtr();

            if (!section) {
                found = false;
                section = &newSection;
            }

            if (token == EToken::Eof) {
                if (!found) {
                    section->first() = std::move(sectionName);
                    content.emplace(std::move(*section));
                }

                return EResult::Ok;
            }

            EXPECT_TOKEN(Eol);

            result = parseSection(stream, size, *section, index, line);

            if (result == EResult::OkEof) {
                if (!found) {
                    section->first() = std::move(sectionName);
                    content.emplace(std::move(*section));
                }

                return EResult::Ok;
            }

            if (result != EResult::Ok)
                return result;

            if (!found) {
                section->first() = std::move(sectionName);
                content.emplace(std::move(*section));
            }
        }
    }

}

#undef EXPECT_EOF
#undef FETCH_NEXT
#undef FETCH_NEXT_EOL
#undef EXPECT_TOKEN
#undef EXPECT_TOKEN2