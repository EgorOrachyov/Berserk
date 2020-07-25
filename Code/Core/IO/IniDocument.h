/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_INIDOCUMENT_H
#define BERSERK_INIDOCUMENT_H

#include <IO/IniValue.h>
#include <Platform/File.h>

namespace Berserk {

    /**
     * @brief Ini document
     *
     * This class represent ini format data, supports reading of the data
     * from ini files, strings, allows access data, manipulate and save in
     * ini format. Primary used for config files representation.
     *
     * @note Supports values, presented as bool, float, int, string, null, and
     *       array of values. Base value type is generic Variant, which is compatible to JsonValue.
     *
     * @note Provides feature of by section values grouping.
     *       Default section is "General" always exist.
     *
     * @note For comments in file use ';' symbol to mark as the comment the rest of the line.
     */
    class IniDocument {
    public:

        static const char DEFAULT_SECTION_NAME[];
        using Section = TMap<CString,IniValue>;
        using Content = TMap<CString,Section>;

        IniDocument() = default;
        IniDocument(File& file);
        IniDocument(const char* string);
        IniDocument(const CString& string);
        IniDocument(const char* string, uint32 length);

        IniDocument(IniDocument&& other) noexcept = default;
        IniDocument& operator=(IniDocument&& other) noexcept = default;

        bool isParsed() const { return mIsParsed; }

        Content& getContent() { return mContent; };
        const Content& getContent() const { return mContent; };

    private:

        bool parse(const char* string, uint32 length);

        bool mIsParsed = false;
        TMap<CString,TMap<CString,IniValue>> mContent;
    };

}


#endif //BERSERK_INIDOCUMENT_H
