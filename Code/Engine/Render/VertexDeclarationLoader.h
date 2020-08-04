/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VERTEXDECLARATIONLOADER_H
#define BERSERK_VERTEXDECLARATIONLOADER_H

#include <IO/JsonValue.h>
#include <RenderResources/VertexDeclaration.h>
#include <RenderResources/VertexDeclarationBuilder.h>

namespace Berserk {
    namespace Render {

        /** Loads declaration from JSON value */
        class VertexDeclarationLoader {
        public:

            VertexDeclarationLoader(JsonValue& value) : mValue(value) {

            }

            void load() {
                VertexDeclarationBuilder builder;
                auto& decl = mValue.getDict();

                builder.setName(decl["Name"].getString());

                auto& buffers = decl["Buffers"].getArray();

                for (auto& bufferVal: buffers) {
                    auto& buffer = bufferVal.getDict();

                    auto& name = buffer["Name"].getString();
                    auto hasIterating = buffer.contains("Iterating");

                    if (hasIterating) {
                        auto iterating = RHIDefinitionsUtil::getIteratingFromString(buffer["Iterating"]);
                        builder.addBuffer(name, iterating);
                    }
                    else {
                        builder.addBuffer(name);
                    }

                    auto& elements = buffer["Elements"].getArray();

                    for (auto& elementVal: elements) {
                        auto& element = elementVal.getDict();
                        auto type = RHIDefinitionsUtil::getElementTypeFromString(element["Type"]);
                        builder.addElement(element["Name"].getString(), type);
                    }
                }

                mInstance = builder.buildShared();
            }

            const TPtrShared<VertexDeclaration>& getDeclaration() const {
                return mInstance;
            }

        private:

            JsonValue& mValue;
            TPtrShared<VertexDeclaration> mInstance;

        };

    }
}

#endif //BERSERK_VERTEXDECLARATIONLOADER_H
