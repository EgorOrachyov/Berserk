/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
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