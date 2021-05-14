/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/RHI/RHIProgramCompilerEnv.hpp>

namespace Berserk {

    void RHIProgramCompilerEnv::SetDefine(String name) {
        DefineInfo info;
        info.name = std::move(name);
        info.type = EDefineType::Flag;
        mDefines.Move(info);
    }

    void RHIProgramCompilerEnv::SetDefine(String name, int32 value) {
        DefineInfo info;
        info.name = std::move(name);
        info.value = std::move(String::ToString(value));
        info.type = EDefineType::Value;
        mDefines.Move(info);
    }

    void RHIProgramCompilerEnv::SetDefine(String name, String value) {
        DefineInfo info;
        info.name = std::move(name);
        info.value = std::move(value);
        info.type = EDefineType::Value;
        mDefines.Move(info);
    }

    void RHIProgramCompilerEnv::AddIncludePath(String path) {
        mIncludePaths.Move(path);
    }

    void RHIProgramCompilerEnv::AddIncludeSource(String includeName, TArray<char> includeText) {
        SourceIncludeInfo info;
        info.name = std::move(includeName);
        info.text = std::move(includeText);
        mSourceIncludes.Move(info);
    }

}