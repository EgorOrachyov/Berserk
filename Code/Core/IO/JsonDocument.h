/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_JSONDOCUMENT_H
#define BERSERK_JSONDOCUMENT_H

#include <IO/JsonValue.h>
#include <Platform/File.h>

namespace Berserk {

    /**
     * @brief Json document
     *
     * This class represent json data, which could be parsed from file or
     * string, dynamically created or updated, and saved in json format.
     *
     * @note This json implementation supports strings as keys and values,
     *       lists of elements, objects, floating point values and signed integer values.
     *
     * @note Internally as implementation relies on Variant type implementation,
     *       which is not fast, but keeps memory usage per value less than one string.
     */
    class JsonDocument {
    public:

        JsonDocument(File& file);
        JsonDocument(const char* string);
        JsonDocument(const CString& string);
        JsonDocument(const char* string, uint32 length);

        bool isParsed() const { return mIsParsed; }

        JsonValue& getContent() { return mValue; };
        const JsonValue& getContent() const { return mValue; };

    private:

        bool parse(const char* string, uint32 length);

        bool mIsParsed = false;
        JsonValue mValue;
    };


}

#endif //BERSERK_JSONDOCUMENT_H