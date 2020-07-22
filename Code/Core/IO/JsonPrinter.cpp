/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/JsonPrinter.h>

namespace Berserk {

    JsonPrinter::JsonPrinter(uint32 preallocate) {
        mBuilderPreallocate = preallocate;
    }
    
    CString JsonPrinter::print(const JsonValue &value) {
        CStringBuilder builder;
        builder.ensureCapacity(mBuilderPreallocate);
        
        print(builder, value);
        return builder.toString();
    }

    CString JsonPrinter::print(const JsonDocument &document) {
        return print(document.getContent());
    }

    void JsonPrinter::print(CStringBuilder &builder, const JsonValue &value) {
        auto current = mIndentation;
        auto& v = (JsonValue&) value;

        switch (value.getType()) {
            case EVariantType::Null:
            case EVariantType::Int:
            case EVariantType::Bool:
            case EVariantType::Float:
            case EVariantType::String: {
                builder.appendN(" ", mValueIndentation);
                value.toStringBuilder(builder);
                return;
            }
            case EVariantType::Array: {
                builder.appendN(" ", mValueIndentation);
                builder.append("[\n");

                auto& array = v.getArray();

                uint32 i = 0;
                for (auto& entry: array) {
                    mIndentation = current + 1;
                    mValueIndentation = current + 1;
                    print(builder, entry);

                    // Insert ',' if it is not last entry in the array
                    if (i + 1 < array.size()) {
                        builder.append(',');
                    }

                    builder.append('\n');
                    i += 1;
                }

                mIndentation = current;
                mValueIndentation = 0;
                builder.appendN(" ", mIndentation);
                builder.append(']');
                return;
            }
            case EVariantType::Map: {
                builder.appendN(" ", mValueIndentation);
                builder.append("{\n");

                auto& object = v.getObject();

                uint32 i = 0;
                for (auto& pair: object) {
                    builder.appendN(" ", current + 1);
                    pair.first().toStringBuilder(builder);
                    builder.append(':');
                    builder.append(' ');

                    mIndentation = current + 1;
                    print(builder, pair.second());

                    // Insert ',' if it is not last entry in the object
                    if (i + 1 < object.size()) {
                        builder.append(",");
                    }

                    builder.append('\n');
                    i += 1;
                }

                mIndentation = current;
                builder.appendN(" ", mIndentation);
                builder.append('}');
            }
            default:
                return;
        }
    }
    
    void JsonPrinter::print(CStringBuilder &builder, const JsonDocument &document) {
        print(builder, document.getContent());
    }
    
}
