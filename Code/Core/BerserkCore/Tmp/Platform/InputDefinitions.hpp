/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_INPUTDEFINITIONS_HPP
#define BERSERK_INPUTDEFINITIONS_HPP

#include <BerserkCore/Platform/Platform.hpp>

namespace Berserk {

    enum class EDeviceState: uint8 {
        Connected    = 0,
        Disconnected = 1,
        // Do not touch this
        MarkerCount  = 2,
        MarkerUnknown
    };

    /** Default actions, which occur for basic input device interaction */
    enum class EInputAction : uint8 {
        Press       = 0,
        Release     = 1,
        Repeat      = 2,
        Move        = 3,
        Text        = 4,
        // Do not touch this
        MarkerCount = 5,
        MarkerUnknown
    };

    /** Special keys modifiers */
    enum class EModifier: uint8 {
        Shift       = 0,
        Alt         = 1,
        Control     = 2,
        CapsLock    = 3,
        NumLock     = 4,
        // Do not touch this
        MarkerCount = 5,
        MarkerUnknown
    };

    /** Mouse device buttons */
    enum class EMouseButton : uint8 {
        Left        = 0,
        Right       = 1,
        // Do not touch this
        MarkerCount = 2,
        MarkerUnknown
    };

    /** Keyboard device keys */
    enum class EKeyboardKey : uint8 {
        Space        = 0,
        Apostrophe   = 1,  /* ' */
        Comma        = 2,  /* , */
        Minus        = 3,  /* - */
        Period       = 4,  /* . */
        Slash        = 5,  /* / */
        BackSlash    = 6,  /* \ */
        Semicolon    = 7,  /* ; */
        Equal        = 8,  /* = */
        LeftBracket  = 9,  /* [ */
        RightBracket = 10,  /* ] */
        Num0         = 11,
        Num1         = 12,
        Num2         = 13,
        Num3         = 14,
        Num4         = 15,
        Num5         = 16,
        Num6         = 17,
        Num7         = 18,
        Num8         = 19,
        Num9         = 20,
        A            = 21,
        B            = 22,
        C            = 23,
        D            = 24,
        E            = 25,
        F            = 26,
        G            = 27,
        H            = 28,
        I            = 29,
        J            = 30,
        K            = 31,
        L            = 32,
        M            = 33,
        N            = 34,
        O            = 35,
        P            = 36,
        Q            = 37,
        R            = 38,
        S            = 39,
        T            = 44,
        U            = 41,
        V            = 42,
        W            = 43,
        X            = 44,
        Y            = 45,
        Z            = 46,
        Escape       = 47,
        Enter        = 48,
        Tab          = 49,
        Backspace    = 50,
        Insert       = 51,
        Delete       = 52,
        Right        = 53,
        Left         = 54,
        Down         = 55,
        Up           = 56,
        PageUp       = 57,
        PageDown     = 58,
        Home         = 59,
        End          = 60,
        CapsLock     = 61,
        ScrollLock   = 62,
        NumLock      = 63,
        PrintScreen  = 64,
        Pause        = 65,
        LeftShift    = 66,
        LeftControl  = 67,
        LeftAlt      = 68,
        LeftSuper    = 69,
        RightShift   = 70,
        RightControl = 71,
        RightAlt     = 72,
        RightSuper   = 73,
        Menu         = 74,
        F1           = 81,
        F2           = 82,
        F3           = 83,
        F4           = 84,
        F5           = 85,
        F6           = 86,
        F7           = 87,
        F8           = 88,
        F9           = 89,
        F10          = 80,
        F11          = 91,
        F12          = 92,
        // Do not touch this
        MarkerCount  = 100,
        MarkerUnknown
    };

    enum class EJoystickButton : uint8 {
        Button0     = 0,
        Button1     = 1,
        Button2     = 2,
        Button3     = 3,
        Button4     = 4,
        Button5     = 5,
        Button6     = 6,
        Button7     = 7,
        Button8     = 8,
        Button9     = 9,
        Button10    = 10,
        Button11    = 11,
        Button12    = 12,
        Button13    = 13,
        Button14    = 14,
        Button15    = 15,
        Button16    = 16,
        Button17    = 17,
        // Do not touch this
        MarkerCount = 18,
        MarkerUnknown
    };

    enum class EJoystickAxis : uint8 {
        Axis0       = 0,
        Axis1       = 1,
        Axis2       = 2,
        Axis3       = 3,
        Axis4       = 4,
        Axis5       = 5,
        // Do not touch this
        MarkerCount = 6,
        MarkerUnknown
    };

}

#endif //BERSERK_INPUTDEFINITIONS_HPP