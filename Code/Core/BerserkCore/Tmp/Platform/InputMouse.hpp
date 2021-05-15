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

#ifndef BERSERK_INPUTMOUSE_HPP
#define BERSERK_INPUTMOUSE_HPP

#include <BerserkCore/Platform/InputDefinitions.hpp>
#include <BerserkCore/String/String.hpp>
#include <BerserkCore/Math/TVecN.hpp>

namespace Berserk {

    class InputMouse {
    public:
        static const uint32 MAX_BUTTONS_COUNT = (uint32)EMouseButton::MarkerCount;
        virtual ~InputMouse() = default;

        virtual const String& GetName() const = 0;
        virtual EInputAction GetButtonAction(EMouseButton button) const = 0;
        virtual Point2i GetPosition() const = 0;
        virtual Point2i GetPositionDelta() const = 0;
        virtual EDeviceState GetState() const = 0;
        virtual bool IsMoved() const = 0;
        virtual bool HasButtonInput() const = 0;
    };

}

#endif //BERSERK_INPUTMOUSE_HPP