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

#ifndef BERSERK_INPUTDEFS_HPP
#define BERSERK_INPUTDEFS_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>

#include <bitset>

BRK_NS_BEGIN

/**
 * @addtogroup platform
 * @{
 */

/**
 * @class InputDeviceState
 * @brief State of the input device (used primary for joysticks)
 */
enum class InputDeviceState {
    Connected,
    Disconnected,
    Unknown
};

/**
 * @class InputAction
 * @brief Default actions, which occur for basic input device interaction
 */
enum class InputAction : uint32 {
    /** Button or key is pressed */
    Press = 0,
    /** Button or key is released */
    Release = 1,
    /** Button or key is repeated (held) */
    Repeat = 2,
    /** Move (axis) moved */
    Move = 3,
    /** Keyboard unicode text input */
    Text = 4,
    /** Unknown */
    Unknown = 0xffffffff
};

/**
 * @class InputModifier
 * @brief Special keys modifiers (used as bit-vector)
 */
enum class InputModifier : uint32 {
    Shift = 0,
    Alt = 1,
    Control = 2,
    CapsLock = 3,
    NumLock = 4
};

/**
 * @class InputModifiers
 * @brief Input modifies mask (as a set of bits)
 */
using InputModifiers = std::bitset<10>;

/**
 * @class InputMouseButton
 * @brief Mouse device buttons
 */
enum class InputMouseButton : uint32 {
    Left = 0,
    Right = 1,
    Unknown = 0xffffffff
};

/**
 * @class InputKeyboardKey
 * @brief Keyboard device keys
 */
enum class InputKeyboardKey : uint32 {
    Space = 0,
    Apostrophe = 1,    /* ' */
    Comma = 2,         /* , */
    Minus = 3,         /* - */
    Period = 4,        /* . */
    Slash = 5,         /* / */
    BackSlash = 6,     /* \ */
    Semicolon = 7,     /* ; */
    Equal = 8,         /* = */
    LeftBracket = 9,   /* [ */
    RightBracket = 10, /* ] */
    Num0 = 11,
    Num1 = 12,
    Num2 = 13,
    Num3 = 14,
    Num4 = 15,
    Num5 = 16,
    Num6 = 17,
    Num7 = 18,
    Num8 = 19,
    Num9 = 20,
    A = 21,
    B = 22,
    C = 23,
    D = 24,
    E = 25,
    F = 26,
    G = 27,
    H = 28,
    I = 29,
    J = 30,
    K = 31,
    L = 32,
    M = 33,
    N = 34,
    O = 35,
    P = 36,
    Q = 37,
    R = 38,
    S = 39,
    T = 44,
    U = 41,
    V = 42,
    W = 43,
    X = 44,
    Y = 45,
    Z = 46,
    Escape = 47,
    Enter = 48,
    Tab = 49,
    Backspace = 50,
    Insert = 51,
    Delete = 52,
    Right = 53,
    Left = 54,
    Down = 55,
    Up = 56,
    PageUp = 57,
    PageDown = 58,
    Home = 59,
    End = 60,
    CapsLock = 61,
    ScrollLock = 62,
    NumLock = 63,
    PrintScreen = 64,
    Pause = 65,
    LeftShift = 66,
    LeftControl = 67,
    LeftAlt = 68,
    LeftSuper = 69,
    RightShift = 70,
    RightControl = 71,
    RightAlt = 72,
    RightSuper = 73,
    Menu = 74,
    F1 = 81,
    F2 = 82,
    F3 = 83,
    F4 = 84,
    F5 = 85,
    F6 = 86,
    F7 = 87,
    F8 = 88,
    F9 = 89,
    F10 = 80,
    F11 = 91,
    F12 = 92,
    Unknown = 0xffffffff
};

/**
 * @class InputJoystickButton
 * @brief Joystick button name
 */
enum class InputJoystickButton : uint32 {
    Button0 = 0,
    Button1 = 1,
    Button2 = 2,
    Button3 = 3,
    Button4 = 4,
    Button5 = 5,
    Button6 = 6,
    Button7 = 7,
    Button8 = 8,
    Button9 = 9,
    Button10 = 10,
    Button11 = 11,
    Button12 = 12,
    Button13 = 13,
    Button14 = 14,
    Button15 = 15,
    Button16 = 16,
    Button17 = 17,
    Unknown = 0xffffffff
};

/**
 * @class InputJoystickAxis
 * @brief Joystick axis id
 */
enum class InputJoystickAxis : uint32 {
    Axis0 = 0,
    Axis1 = 1,
    Axis2 = 2,
    Axis3 = 3,
    Axis4 = 4,
    Axis5 = 5,
    Unknown = 0xffffffff
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_INPUTDEFS_HPP
