/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
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