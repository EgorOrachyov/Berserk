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

#ifndef BERSERK_BERSERK_HPP
#define BERSERK_BERSERK_HPP

/**
 * @defgroup core
 * @brief Core functionality of the engine
 *
 * Provides access to engine core functionality, used among others
 * runtime modules. Defines essential engine building blocks, such
 * as strings manipulation and encoding, events, config, io,
 * engine globals access, such as event dispatcher, scheduler,
 * platform manager, and etc.
 */

#include <core/Config.hpp>
#include <core/Data.hpp>
#include <core/Engine.hpp>
#include <core/EventDispatcher.hpp>
#include <core/Scheduler.hpp>
#include <core/Thread.hpp>
#include <core/Typedefs.hpp>
#include <core/UUID.hpp>
#include <core/event/Event.hpp>
#include <core/event/EventDropInput.hpp>
#include <core/event/EventJoystick.hpp>
#include <core/event/EventKeyboard.hpp>
#include <core/event/EventMouse.hpp>
#include <core/event/EventWindow.hpp>
#include <core/image/Image.hpp>
#include <core/image/ImageUtil.hpp>
#include <core/io/Config.hpp>
#include <core/io/Logger.hpp>
#include <core/math/MathUtils.hpp>
#include <core/math/MathUtils2d.hpp>
#include <core/math/MathUtils3d.hpp>
#include <core/math/TMatMxN.hpp>
#include <core/math/TQuat.hpp>
#include <core/math/TVecN.hpp>
#include <core/string/String.hpp>
#include <core/string/String16u.hpp>
#include <core/string/StringName.hpp>
#include <core/string/Unicode.hpp>
#include <core/templates/Ref.hpp>
#include <core/templates/RefCnt.hpp>

/**
 * @defgroup platform
 * @brief Platform specific functionality abstraction
 *
 * Provides unified and platform agnostic access to os specific
 * objects, such as file system access, windows and input management and
 * game application core structure. Use this module to access
 * platform specific features.
 */

#include <platform/Application.hpp>
#include <platform/FileSystem.hpp>
#include <platform/Input.hpp>
#include <platform/InputDefs.hpp>
#include <platform/InputDevices.hpp>
#include <platform/Output.hpp>
#include <platform/Window.hpp>
#include <platform/WindowManager.hpp>

/**
 * @defgroup rhi
 * @brief Rendering hardware interface
 *
 * Provides unified interface for the rendering hardware, required
 * for lower level rendering. Abstracts such primitives, as texture,
 * sample, shader, framebuffer, render pass, pipeline etc.
 *
 * Interface is designed as verbose API with minimum number of
 * global state variables, requires explicit pre-creation of rendering objects.
 *
 * Intended for implementation by OpenGL and Vulkan APIs.
 */

#include <rhi/RHIBuffer.hpp>
#include <rhi/RHICommandList.hpp>
#include <rhi/RHIDefs.hpp>
#include <rhi/RHIDevice.hpp>
#include <rhi/RHIFramebuffer.hpp>
#include <rhi/RHIGraphicsPipeline.hpp>
#include <rhi/RHIRenderPass.hpp>
#include <rhi/RHIResource.hpp>
#include <rhi/RHIResourceSet.hpp>
#include <rhi/RHISampler.hpp>
#include <rhi/RHIShader.hpp>
#include <rhi/RHITexture.hpp>
#include <rhi/RHIVertexDeclaration.hpp>

/**
 * @defgroup render
 * @brief Rendering module of the engine
 *
 * Provides access to engine low-level and high-level rendering
 * functionality, required for the rendering of the complex 2d/3d
 * scenes as well as auxiliary features for debugging.
 *
 * Implements such primitives as high-level shaders, materials,
 * provides low-level abstraction for render view, render objects,
 * auxiliary debug draw features.
 *
 * Also provides abstraction for 3d scene, high-level render objects,
 * meshes, post effects, high-level graphics pipelines.
 */

#include <render/RenderEngine.hpp>
#include <render/material/Material.hpp>
#include <render/material/MaterialParams.hpp>
#include <render/mesh/Mesh.hpp>
#include <render/mesh/MeshFormats.hpp>
#include <render/shader/Shader.hpp>
#include <render/shader/ShaderManager.hpp>
#include <render/shader/ShaderPass.hpp>
#include <render/shader/ShaderTechnique.hpp>

/**
 * @defgroup resource
 * @brief Resources management module
 *
 * Module provides access to engine resources.
 * Defines base resource class, provides resource manager class
 * for safe synchronized/async resources loading and importing.
 *
 * Also defines generic importer interface and import options,
 * which allows to load or import arbitrary supported engine resources.
 *
 * All common built-in engine resources, such as Material, Shader,
 * Texture, Mesh and etc. are defined in this module. Class names
 * uses special `Res` prefix, to distinguish them from other classes.
 */

#include <resource/ResTexture.hpp>
#include <resource/Resource.hpp>
#include <resource/ResourceImporter.hpp>
#include <resource/ResourceManager.hpp>

#endif//BERSERK_BERSERK_HPP
