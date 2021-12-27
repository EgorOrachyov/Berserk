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

#ifndef BERSERK_SHADERMANAGER_HPP
#define BERSERK_SHADERMANAGER_HPP

#include <core/Config.hpp>
#include <core/Crc32.hpp>
#include <core/Typedefs.hpp>
#include <render/shader/Shader.hpp>
#include <render/shader/ShaderArchetype.hpp>

#include <unordered_map>

BRK_NS_BEGIN

/**
 * @addtogroup render
 * @{
 */

/**
 * @class ShaderManager
 * @brief
 */
class ShaderManager {
public:
    using ArchetypePtr = std::shared_ptr<ShaderArchetype>;

    BRK_API ShaderManager();
    BRK_API ~ShaderManager() = default;

    /**
     * @brief Attempts to load shader with specified filepath and defines set
     *
     * @paragraph
     *  Uses specified filepath to load engine shader file from the disc.
     *  Provided defines will be used to compile desired shader variation.
     *
     * @paragraph
     *  If shader with the same path and variation already compiled and
     *  cached, the manager will return cached instance handler.
     *  Otherwise new instance must be compiled and cached.
     *
     * @param filepath Path to shader file; might be relative to game resource folder
     * @param options Set of options to pass to the shader
     *
     * @return Shader reference; null if failed load
     */
    BRK_API Ref<const Shader> Load(const String &filepath, const Ref<ShaderCompileOptions> &options);

    BRK_API bool IsRegistered(const StringName &archetype) const;
    BRK_API void RegisterArchetype(ArchetypePtr archetypePtr);
    BRK_API ArchetypePtr FindArchetype(const StringName &archetype) const;

private:
    /** Registered archetypes */
    std::unordered_map<StringName, ArchetypePtr> mArchetypes;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_SHADERMANAGER_HPP
