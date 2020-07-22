/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_JSONPRINTER_H
#define BERSERK_JSONPRINTER_H

#include <IO/JsonDocument.h>
#include <String/CString.h>
#include <String/CStringBuilder.h>

namespace Berserk {

    /**
     * @brief Json printer
     *
     * Writes json data in human-readable form with indentations to strings
     * and string builders.
     */
    class JsonPrinter final {
    public:

        static const uint32 BUILDER_PREALLOCATE = 128;

        JsonPrinter() = default;
        JsonPrinter(uint32 preallocate);

        /** @return Pretty printed json to the string */
        CString print(const JsonValue& value);

        /** @return Pretty printed json to the string */
        CString print(const JsonDocument& document);

        /**
         * Print json to the builder, passed as an argument
         * @param builder To write json data
         * @param value Actual json data to print
         */
        void print(CStringBuilder& builder, const JsonValue& value);

        /**
         * Print json to the builder, passed as an argument
         * @param builder To write json data
         * @param document Actual json data to print
         */
        void print(CStringBuilder& builder, const JsonDocument& document);

    private:

        uint32 mBuilderPreallocate = BUILDER_PREALLOCATE;
        uint32 mIndentation = 0;
        uint32 mValueIndentation = 0;

    };

}

#endif //BERSERK_JSONPRINTER_H