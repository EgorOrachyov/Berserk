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
#include <core/event/Event.hpp>
#include <core/event/EventDropInput.hpp>
#include <core/event/EventJoystick.hpp>
#include <core/event/EventKeyboard.hpp>
#include <core/event/EventMouse.hpp>
#include <core/event/EventWindow.hpp>
#include <core/image/Image.hpp>
#include <core/image/ImageUtil.hpp>
#include <core/io/Logger.hpp>
#include <core/math/MathUtils.hpp>
#include <core/math/MathUtils2d.hpp>
#include <core/math/MathUtils3d.hpp>
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

#endif//BERSERK_BERSERK_HPP
