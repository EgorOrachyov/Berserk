/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHIPROGRAMCOMPILERENV_HPP
#define BERSERK_RHIPROGRAMCOMPILERENV_HPP

#include <BerserkCore/Containers/TArray.hpp>
#include <BerserkCore/String/String.hpp>

namespace Berserk {

    /**
     * @brief Program compiler environment
     *
     * Allows to modify compiler context in order to add at run-time
     * preprocessor definitions, include paths and include sources to
     * the shader source.
     *
     * Allowed only for programs, which compiled form text-based source code.
     */
    class RHIProgramCompilerEnv {
    public:

        enum class EDefineType {
            /** Will be simply defined */
            Flag,
            /** Will be placed as-is */
            Value,
            /** Will be wrapped in quotes */
            String
        };

        /** Define data */
        struct DefineInfo {
            String name;
            String value;
            EDefineType type;
        };

        /** Text include data */
        struct SourceIncludeInfo {
            String name;
            TArray<char> text;
        };

        RHIProgramCompilerEnv() = default;
        RHIProgramCompilerEnv(RHIProgramCompilerEnv&& other) noexcept = default;
        ~RHIProgramCompilerEnv() = default;

        void SetDefine(String name);
        void SetDefine(String name, int32 value);
        void SetDefine(String name, String value);
        void AddIncludePath(String path);
        void AddIncludeSource(String includeName, TArray<char> includeText);

        const TArray<DefineInfo>& GetDefines() const { return mDefines; };
        const TArray<String>& GetIncludePaths() const { return mIncludePaths; }
        const TArray<SourceIncludeInfo>& GetSourceIncludes() const { return mSourceIncludes; }

    private:
        TArray<DefineInfo> mDefines;
        TArray<String> mIncludePaths;
        TArray<SourceIncludeInfo> mSourceIncludes;
    };

}

#endif //BERSERK_RHIPROGRAMCOMPILERENV_HPP