/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_INPUTDEVICES_H
#define BERSERK_INPUTDEVICES_H

#include <Math/Point2i.h>
#include <String/CString.h>
#include <Containers/TArray.h>
#include <Platform/InputDefinitions.h>

namespace Berserk {

    /** Represent connected mouse state */
    class Mouse {
    public:
        static const uint32 MAX_BUTTONS_COUNT = 2;

        virtual ~Mouse() = default;

        /** @return Optional keyboard name */
        virtual const CString& getName() const = 0;

        /** @return Action on keyboard */
        virtual EInputAction getButtonAction(EMouseButton button) const = 0;

        /** @return Mouse position */
        virtual Point2i getPosition() const = 0;

        /** @return Mouse position delta (since last input update cycle) */
        virtual Point2i getPositionDelta() const = 0;

        /** @return True if mouse was moved (since last input update cycle) */
        virtual bool isMoved() const = 0;

        /** @return True if has action on some button (since last input update cycle) */
        virtual bool hasButtonInput() const = 0;

        /** @return Keyboard state */
        virtual EDeviceState getState() const = 0;
    };

    /** Represent connected keyboard state */
    class Keyboard {
    public:
        static const uint32 MAX_KEYS_COUNT = 100;

        virtual ~Keyboard() = default;

        /** @return Optional keyboard name */
        virtual const CString& getName() const = 0;

        /** Query text input code points */
        virtual void getTextInput(TArray<wchar>& codePoints) const = 0;

        /** @return True if has text input */
        virtual bool hasTextInput() const = 0;

        /** @return Action on keyboard */
        virtual EInputAction getKeyAction(EKeyboardKey key) const = 0;

        /** @return True if has action on keys */
        virtual bool hasKeyInput() const = 0;

        /** @return Modifiers state */
        virtual ModifiersMask getModifiersMask() const = 0;

        /** @return Keyboard state */
        virtual EDeviceState getState() const = 0;
    };

    /** Represent connected joystick state */
    class Joystick {
    public:
        static const uint32 MAX_BUTTONS_COUNT = 20;
        static const uint32 MAX_AXES_COUNT = 10;

        virtual ~Joystick() = default;

        /** @return Number of axes joystick has */
        virtual uint32 getAxesCount() const = 0;

        /** @return Number of buttons joystick has */
        virtual uint32 getButtonsCount() const = 0;

        /** @return Optional joystick name */
        virtual const CString& getName() const = 0;

        /** @return Optional joystick guid */
        virtual const CString& getGuid() const = 0;

        /** @return Value of the axis */
        virtual float getAxisValue(EJoystickAxis axis) const = 0;

        /** @return True if has action on some button (since last input update cycle) */
        virtual bool hasButtonInput() const = 0;

        /** @return True if joystick currently connected */
        virtual bool isConnected() const = 0;

        /** @return Action on button */
        virtual EInputAction getButtonAction(EJoystickButton button) const = 0;

        /** @return Joystick state */
        virtual EDeviceState getState() const = 0;
    };

}

#endif //BERSERK_INPUTDEVICES_H
