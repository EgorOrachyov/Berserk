/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ShaderDefsMacro.h>

namespace Berserk {
    namespace Render {

        void ShaderDefsMacro::setDefinitionPrefix(const char *prefix) {
            mDefPrefix = prefix;
        }

        void ShaderDefsMacro::addDefinition(const char *name, const char *body) {
            Definition def;
            def.first() = name;
            def.second() = body;
            mDefinitions.move(def);
        }

        void ShaderDefsMacro::writeDefs(CStringBuilder &builder) const {
            for (auto &def: mDefinitions) {
                builder.append(mDefPrefix);
                builder.append(' ');
                builder.append(def.first());
                builder.append(' ');
                builder.append(def.second());
                builder.append('\n');
            }
        }

    }
}