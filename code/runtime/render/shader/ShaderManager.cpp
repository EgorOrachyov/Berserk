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

#include <core/Engine.hpp>
#include <core/io/Logger.hpp>
#include <render/shader/ShaderManager.hpp>

BRK_NS_BEGIN

Ref<const Shader> ShaderManager::Load(const String &filepath, const ShaderCompileOptions &options) {
    auto &engine = Engine::Instance();
    auto &fs = engine.GetFileSystem();
    auto fullPath = fs.GetFullFilePath(filepath);

    // If no path, nothing to do
    if (fullPath.empty()) {
        BRK_ERROR("Failed to get full path for file=\"" << filepath << "\"");
        return Ref<const Shader>();
    }

    // Check cache first
    ShaderCacheKey key{};
    key.path = fullPath;
    key.options = options;
    auto query = mCachedShaders.find(key);
    if (query != mCachedShaders.end()) {
        auto &entry = query->second;
        entry.used += 1;
        return entry.shader.As<const Shader>();
    }

    // Compile from scratch

    return Ref<const Shader>();
}

bool ShaderManager::IsRegistered(const StringName &archetype) const {
    auto query = mArchetypes.find(archetype);
    return query != mArchetypes.end();
}

void ShaderManager::RegisterArchetype(ArchetypePtr archetypePtr) {
    assert(archetypePtr);
    const auto &name = archetypePtr->GetArchetype();
    if (IsRegistered(name)) {
        BRK_ERROR("Archetype with name=\"" << name << "\" already registered");
        return;
    }
    mArchetypes.emplace(name, std::move(archetypePtr));
}

BRK_NS_END