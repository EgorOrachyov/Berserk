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

#ifndef BERSERK_RENDERENGINE_HPP
#define BERSERK_RENDERENGINE_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>

#include <render/mesh/MeshFormats.hpp>
#include <render/shader/ShaderManager.hpp>

#include <memory>

BRK_NS_BEGIN

/**
 * @addtogroup render
 * @{
 */

/**
 * @class RenderEngine
 * @brief Main rendering engine class
 */
class RenderEngine {
public:
    BRK_API RenderEngine() = default;
    BRK_API ~RenderEngine() = default;

    /** @return Mesh formats manager */
    BRK_API MeshFormats &GetMeshFormats() const;

    /** @return Shader manager */
    BRK_API ShaderManager &GetShaderManager() const;

private:
    friend class Engine;

    BRK_API void Init();
    BRK_API void PreUpdate();
    BRK_API void PostUpdate();

private:
    std::unique_ptr<MeshFormats> mMeshFormats;     /** Mesh formats manager */
    std::unique_ptr<ShaderManager> mShaderManager; /** Shader manager */
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RENDERENGINE_HPP
