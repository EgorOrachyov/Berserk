/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/Shaders/ShaderProfile.h>
#include <IO/Ini.h>

namespace Berserk {
    namespace Rendering {

        ShaderProfile::ShaderProfile(CString profileName, ERenderDeviceType deviceType, const Ini &config) {
            mProfileName = std::move(profileName);
            mDeviceType = deviceType;

            auto toLoad = System::getDeviceTypeAsString(mDeviceType);
            auto section = config.getSection(toLoad);

            if (section.isNotNull()) {
                auto lang = config.getValue(*section, "ShadingLanguage");

                if (lang.isNotNull()) {
                    mLanguage = RHIDefinitionsUtil::getLanguageFromString(*lang);

                    for (const auto& entry: (*section).second()) {
                        if (entry.first() != "ShadingLanguage")
                            mValues.add(entry.first(), entry.second());
                    }
                }
            }
        }

        void ShaderProfile::findEntriesWithPrefix(const CString &prefix, TArrayStatic<TRef<const Entry>> &entries) const {
            for (const auto& e: mValues) {
                if (e.first().starts(prefix.data())) {
                    entries.emplace(&e);
                }
            }
        }

    }
}