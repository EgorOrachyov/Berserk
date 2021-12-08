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

#ifndef BERSERK_WINDOWMANAGER_HPP
#define BERSERK_WINDOWMANAGER_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/templates/Ref.hpp>
#include <platform/Window.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class WindowManager
 * @brief Manager for OS windows
 */
class WindowManager {
public:
    BRK_API virtual ~WindowManager() = default;

    /**
     * @brief Create new OS native rendering window
     *
     * Creates new window object with specified properties.
     * Automatically initializes rendering context of window, so it is ready for actual rendering.
     *
     * @note Must be called on the main (game) thread

     * @param name Unique name of the window in the engine
     * @param size Size of the window in units
     * @param title Window title to set
     *
     * @return Created window; null if failed to create or if name is not unique
     */
    BRK_API virtual Ref<Window> CreateWindow(const StringName &name, const Size2i &size, const String &title) = 0;

    /** @return Returns primary window of the engine */
    BRK_API virtual Ref<Window> GetPrimaryWindow() = 0;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_WINDOWMANAGER_HPP
