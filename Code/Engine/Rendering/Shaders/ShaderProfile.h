/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADERPROFILE_H
#define BERSERK_SHADERPROFILE_H

#include <Containers/TMap.h>
#include <Containers/TArrayStatic.h>
#include <RHI/RHIDefinitions.h>
#include <Platform/ISystem.h>
#include <IO/Ini.h>

namespace Berserk {
    namespace Rendering {

        /** Represents mapping to load specific rendering shaders from RHI platform sources */
        class ShaderProfile final {
        public:
            using Entry = TPair<CString,CString>;

            /**
             * Create shader profile map from specified profile name for specified platform
             * @param profileName Name of the profile (actual file must be [profileName].ini)
             * @param deviceType Type of the target device to load profile
             * @param config Parsed ini file with profile data
             */
            ShaderProfile(CString profileName, ERenderDeviceType deviceType, const Ini &config);
            ShaderProfile() = default;
            ~ShaderProfile() = default;

            bool contains(const CString& name) const { return mValues.contains(name); }
            TRef<const CString> find(const CString& name) const { return mValues.getPtr(name); }
            void findEntriesWithPrefix(const CString &prefix, TArrayStatic<TRef<const Entry>> &entries) const;

            EShaderLanguage getLanguage() const { return mLanguage; }
            ERenderDeviceType getDeviceType() const { return mDeviceType; }
            const CString &getProfileName() const { return mProfileName; }
            const TMap<CString,CString> getMappings() const { return mValues; }

        private:
            EShaderLanguage mLanguage = EShaderLanguage::Undefined;
            ERenderDeviceType mDeviceType = ERenderDeviceType::Undefined;
            CString mProfileName;
            TMap<CString,CString> mValues;
        };

    }
}

#endif //BERSERK_SHADERPROFILE_H