/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ShaderDefinitions.h>

namespace Berserk {
    namespace Render {

        ShaderDefinitions* ShaderDefinitions::gShaderDefinitions = nullptr;

        ShaderDefinitions::ShaderDefinitions() {
            BERSERK_COND_ERROR_RET(gShaderDefinitions == nullptr, "Allowed only single ShaderDefinitions instance");
            gShaderDefinitions = this;
        }

        ShaderDefinitions::~ShaderDefinitions() {
            gShaderDefinitions = nullptr;
        }

        void ShaderDefinitions::addOrReplaceDefinition(const CString &name, Variant value, DefinitionMask mask) {
            DefinitionEntry entry;
            entry.value = std::move(value);
            entry.mask = mask;

            CString key = name;
            mDefinitions.move(key, entry);
        }

        bool ShaderDefinitions::containsDefinition(const CString &name) {
            return mDefinitions.contains(name);
        }

        const Variant& ShaderDefinitions::getDefinition(const CString &name) const {
            auto ptr = mDefinitions.getPtr(name);
            BERSERK_COND_ERROR_FAIL(ptr.isNotNull(), "No such definitions: %s", name.data());

            return ptr->value;
        }

        const DefinitionMask& ShaderDefinitions::getDefinitionMask(const CString &name) const {
            auto ptr = mDefinitions.getPtr(name);
            BERSERK_COND_ERROR_FAIL(ptr.isNotNull(), "No such definitions: %s", name.data());

            return ptr->mask;
        }

        ShaderDefinitions& ShaderDefinitions::getSingleton() {
            return *gShaderDefinitions;
        }

    }
}