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

#ifndef BERSERK_WINDOW_HPP
#define BERSERK_WINDOW_HPP

#include <core/Config.hpp>
#include <core/math/TVecN.hpp>
#include <core/string/String.hpp>
#include <core/string/StringName.hpp>
#include <core/templates/RefCnt.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup platform
 * @{
 */

/**
 * @class Window
 * @brief Platform specific OS window for rendering
 */
class Window : public RefCnt {
public:
    BRK_API ~Window() override = default;

    /** @return Current window position */
    BRK_API virtual Point2i GetPosition() const = 0;

    /** @return Window size */
    BRK_API virtual Size2i GetSize() const = 0;

    /** @return Window framebuffer size */
    BRK_API virtual Size2i GetFramebufferSize() const = 0;

    /** @return Ratio of pixels per window size units */
    BRK_API virtual Vec2f GetPixelRatio() const = 0;

    /** @return Aspect ratio width / height */
    BRK_API virtual float GetAspectRatio() const {
        auto s = GetSize();
        return s[1] > 0 ? static_cast<float>(s[0]) / static_cast<float>(s[0]) : 1.0f;
    }

    /** @return True if window receives focus */
    BRK_API virtual bool IsInFocus() const = 0;

    /** @return True if window is closed */
    BRK_API virtual bool IsClosed() const = 0;

    /** @return Window title */
    BRK_API virtual String GetTitle() const = 0;

    /** @return Window unique name for look-up */
    BRK_API virtual StringName GetName() const = 0;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_WINDOW_HPP
