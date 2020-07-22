/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/IniDocument.h>
#include <IO/IniParser.h>
#include <Containers/BinaryData.h>

namespace Berserk {

    const char IniDocument::DEFAULT_SECTION_NAME[] = "General";

    IniDocument::IniDocument(File &file) {
        uint32 fileSize = file.getSize();
        BinaryData data;
        data.resize(fileSize);
        EError result = file.read(data.data(), fileSize);

        BERSERK_COND_ERROR_RET(result == EError::OK, "Failed to read data content");
        BERSERK_COND_ERROR(parse((const char*) data.data(), fileSize), "Failed parse file");
    }

    IniDocument::IniDocument(const char *string) {
        parse(string, CStringUtility::length(string));
    }

    IniDocument::IniDocument(const CString &string) {
        parse(string.data(), string.length());
    }

    IniDocument::IniDocument(const char *string, uint32 length) {
        parse(string, length);
    }

    bool IniDocument::parse(const char *string, Berserk::uint32 length) {
        IniParser parser(mContent, string, length);
        mIsParsed = parser.parse() == IniParser::EResult::Ok;
        return mIsParsed;
    }    
    
}