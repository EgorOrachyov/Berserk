/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_INPUTJOYSTICK_HPP
#define BERSERK_INPUTJOYSTICK_HPP

#include <BerserkCore/Platform/InputDefinitions.hpp>
#include <BerserkCore/String/String.hpp>
#include <BerserkCore/Containers/TArray.hpp>

namespace Berserk {

    class InputJoystick {
    public:
        static const uint32 MAX_BUTTONS_COUNT = (uint32)EJoystickButton::MarkerCount;
        static const uint32 MAX_AXES_COUNT = (uint32)EJoystickAxis::MarkerCount;
        virtual ~InputJoystick() = default;

        virtual uint32 GetAxesCount() const = 0;
        virtual uint32 GetButtonsCount() const = 0;
        virtual const String& GetName() const = 0;
        virtual const String& GetGuid() const = 0;
        virtual float GetAxisValue(EJoystickAxis axis) const = 0;
        virtual bool HasButtonInput() const = 0;
        virtual bool IsConnected() const = 0;
        virtual EInputAction GetButtonAction(EJoystickButton button) const = 0;
        virtual EDeviceState GetState() const = 0;
    };

}

#endif //BERSERK_INPUTJOYSTICK_HPP