/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADERDEFSMACRO_H
#define BERSERK_SHADERDEFSMACRO_H

#include <TPair.h>
#include <Containers/TArray.h>
#include <String/CString.h>
#include <String/CStringBuilder.h>

namespace Berserk {
    namespace Rendering {

        /**
         * @brief Shader macro preprocessor
         *
         * Macro preprocessor, which allows to add some definitions to the shader source code
         */
        class ShaderDefsMacro {
        public:

            using Definition = TPair<CString, CString>;

            /** Sets def prefix */
            void setDefinitionPrefix(const char *prefix = "#define");

            /** Adds definition entry */
            void addDefinition(const char *name, const char *body = "");

            /** Forms definitions list in string */
            void writeDefs(CStringBuilder &builder) const;

            /** @return Explicitly inserted prefix for each definition in source code */
            const CString &getDefinitionPrefix() const { return mDefPrefix; }

            /** @return Definitions list */
            const TArray<Definition> &getDefinitions() const { return mDefinitions; }

        private:
            CString mDefPrefix;
            TArray<Definition> mDefinitions;
        };

    }
}

#endif //BERSERK_SHADERDEFSMACRO_H