/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_INPUTEVENTS_H
#define BERSERK_INPUTEVENTS_H

#include <TPtrShared.h>
#include <Math/Point2i.h>
#include <Containers/TArray.h>
#include <Platform/InputDevices.h>
#include <Platform/InputDefinitions.h>

namespace Berserk {

    struct InputEventMouse {
        EInputAction inputAction = EInputAction::Unknown;
        EMouseButton mouseButton = EMouseButton::Unknown;
        Point2i position;
        Point2i delta;
        ModifiersMask modifiersMask = 0;

        bool moved() const { return inputAction == EInputAction::Move; }
        bool pressed() const { return inputAction == EInputAction::Press; }
        bool released() const { return inputAction == EInputAction::Release; }
    };

    struct InputEventKeyboard {
        EInputAction inputAction = EInputAction::Unknown;
        EKeyboardKey keyboardKey = EKeyboardKey::Unknown;
        ModifiersMask modifiersMask = 0;
        wchar codepoint = 0;

        bool text() const { return inputAction == EInputAction::Text; }
        bool pressed() const { return inputAction == EInputAction::Press; }
        bool repeated() const { return inputAction == EInputAction::Repeat; }
        bool released() const { return inputAction == EInputAction::Release; }
    };

    struct InputEventJoystick {
        EInputAction inputAction = EInputAction::Unknown;
        EJoystickAxis axis = EJoystickAxis::Unknown;
        EJoystickButton button = EJoystickButton::Unknown;
        float value = 0.0f;
        TPtrShared<Joystick> joystick;

        bool moved() const { return inputAction == EInputAction::Move; }
        bool pressed() const { return inputAction == EInputAction::Press; }
        bool released() const { return inputAction == EInputAction::Release; }
    };

    struct InputEventDrop {
        TArray<CString> values;
    };

}

#endif //BERSERK_INPUTEVENTS_H
