/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADERDEFINITIONS_H
#define BERSERK_SHADERDEFINITIONS_H

#include <Containers/TMap.h>
#include <Containers/TArray.h>
#include <TEnumMask.h>
#include <Variant.h>

namespace Berserk {
    namespace Render {

        /** Flags, which shows, when the definition is inserted */
        enum class EDefinitionFlag {
            General,
            PostProcess,
            Material
        };

        using DefinitionMask = TEnumMask<EDefinitionFlag>;

        /** Stores all the engine shading settings, which are passed as definitions to the shaders code */
        class ShaderDefinitions {
        public:

            struct DefinitionEntry {
                Variant value;
                DefinitionMask mask;
            };

            ShaderDefinitions();
            ~ShaderDefinitions();

            /** Adds or replaces definition with provided name, value and flags */
            void addOrReplaceDefinition(const CString& name, Variant value, DefinitionMask mask = { EDefinitionFlag::General });

            /** @return True if contains definition with specified name */
            bool containsDefinition(const CString& name);

            /** @return Definition value */
            const Variant& getDefinition(const CString& name) const;

            /** @return Definition mask */
            const DefinitionMask& getDefinitionMask(const CString& name) const;

            /** @return Definitions as map */
            const TMap<CString,DefinitionEntry>& getDefinitions() const { return mDefinitions; }

            /** @return Globally accessible shader definitions map */
            static ShaderDefinitions& getSingleton();

        private:

            TMap<CString,DefinitionEntry> mDefinitions;

            static ShaderDefinitions* gShaderDefinitions;

        };

    }
}

#endif //BERSERK_SHADERDEFINITIONS_H