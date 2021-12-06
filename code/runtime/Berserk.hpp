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
#include <core/Typedefs.hpp>
#include <core/event/Event.hpp>
#include <core/io/Logger.hpp>
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

#endif//BERSERK_BERSERK_HPP
