/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_INPUTMANAGER_HPP
#define BERSERK_INPUTMANAGER_HPP

#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/Platform/InputMouse.hpp>
#include <BerserkCore/Platform/InputJoystick.hpp>
#include <BerserkCore/Platform/InputKeyboard.hpp>
#include <BerserkCore/TPtrShared.hpp>
#include <BerserkCore/TMask.hpp>

namespace Berserk {

    struct InputMouseEvent {
        EInputAction action = EInputAction::MarkerUnknown;
        EMouseButton button = EMouseButton::MarkerUnknown;
        Point2i position;
        Point2i delta;
        TMask<EModifier> modifiersMask;
        TPtrShared<InputMouse> mouse;
    };

    struct InputKeyboardEvent {
        EInputAction action = EInputAction::MarkerUnknown;
        EKeyboardKey key = EKeyboardKey::MarkerUnknown;
        TMask<EModifier> modifiersMask;
        uint32 codepoint = 0;
        TPtrShared<InputKeyboard> keyboard;
    };

    struct InputJoystickEvent {
        EInputAction inputAction = EInputAction::MarkerUnknown;
        EJoystickAxis axis = EJoystickAxis::MarkerUnknown;
        EJoystickButton button = EJoystickButton::MarkerUnknown;
        float value = 0.0f;
        TPtrShared<InputJoystick> joystick;
    };

    struct InputDropEvent {
        TArray<String> values;
    };

    class InputMouseListener {
    public:
        virtual ~InputMouseListener() = default;
        virtual void OnMouseInput(const InputMouseEvent& event) = 0;
    };

    class InputKeyboardListener {
    public:
        virtual ~InputKeyboardListener() = default;
        virtual void OnKeyboardInput(const InputKeyboardEvent& event) = 0;
    };

    class InputJoystickListener {
    public:
        virtual ~InputJoystickListener() = default;
        virtual void OnJoystickInput(const InputJoystickEvent& event) = 0;
    };

    /** Handel platform specific input functionality */
    class InputManager {
    public:
        virtual void AddMouseListener(InputMouseListener& listener) = 0;
        virtual void AddKeyboardListener(InputKeyboardListener& listener) = 0;
        virtual void AddJoystickListener(InputJoystickListener& listener) = 0;
        virtual void RemoveMouseListener(InputMouseListener& listener) = 0;
        virtual void RemoveKeyboardListener(InputKeyboardListener& listener) = 0;
        virtual void RemoveJoystickListener(InputJoystickListener& listener) = 0;
        virtual void DisableDispatching() = 0;
        virtual void EnableDispatching() = 0;
        virtual const TPtrShared<InputMouse> &GetMouse() const = 0;
        virtual const TPtrShared<InputKeyboard> &GetKeyboard() const = 0;
        virtual const TArray<TPtrShared<InputJoystick>> &GetJoysticks() const = 0;
    };

}

#endif //BERSERK_INPUTMANAGER_HPP