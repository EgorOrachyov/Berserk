/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/JsonDocument.h>
#include <IO/JsonParser.h>
#include <Containers/BinaryData.h>

namespace Berserk {

    JsonDocument::JsonDocument(File &file) {
        uint32 fileSize = file.getSize();
        BinaryData data;
        data.resize(fileSize);
        EError result = file.read(data.data(), fileSize);

        BERSERK_COND_ERROR_RET(result == EError::OK, "Failed to read data content");

        parse((const char*) data.data(), fileSize);
    }

    JsonDocument::JsonDocument(const char *string) {
        parse(string, CStringUtility::length(string));
    }

    JsonDocument::JsonDocument(const CString &string) {
        parse(string.data(), string.length());
    }

    JsonDocument::JsonDocument(const char *string, uint32 length) {
        parse(string, length);
    }

    bool JsonDocument::parse(const char *string, Berserk::uint32 length) {
        JsonParser parser(mValue, string, length);
        mIsParsed = parser.parse() == JsonParser::EResult::Ok;
    }

}