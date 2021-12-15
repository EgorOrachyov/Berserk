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

#ifndef BERSERK_INPUTDEVICES_HPP
#define BERSERK_INPUTDEVICES_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/math/TVecN.hpp>
#include <core/string/StringName.hpp>
#include <core/templates/RefCnt.hpp>
#include <platform/InputDefs.hpp>

#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup platform
 * @{
 */

/**
 * @class Mouse
 * @brief Mouse device state
 */
class Mouse : public RefCnt {
public:
    static const uint32 MAX_BUTTONS = 2;

    BRK_API ~Mouse() override = default;

    /** @return Mouse position */
    const Point2f &GetPosition() const { return mPosition; }

    /** @return Mouse prev-frame delta */
    const Size2f &GetDelta() const { return mDelta; }

    /** @return Name of this device */
    const StringName &GetName() const { return mName; }

    /** @return Current device state */
    InputDeviceState GetState() const { return mState; }

    /** @return Current states of device buttons */
    const std::vector<InputAction> &GetButtonsStates() const { return mButtons; }

protected:
    StringName mName;
    Point2f mPosition{};
    Size2f mDelta{};
    InputDeviceState mState;
    std::vector<InputAction> mButtons;
};

/**
 * @class Keyboard
 * @brief Keyboard device state
 */
class Keyboard : public RefCnt {
public:
    static const uint32 MAX_KEYS = 100;

    BRK_API ~Keyboard() override = default;

    /** @return Name of this device */
    const StringName &GetName() const { return mName; }

    /** @return Current device state */
    InputDeviceState GetState() const { return mState; }

    /** @return Current states of device keys */
    const std::vector<InputAction> &GetKeysStates() const { return mKeys; }

protected:
    StringName mName;
    InputDeviceState mState;
    std::vector<InputAction> mKeys;
};

/**
 * @class Joystick
 * @brief Joystick (gamepad) device state
 */
class Joystick : public RefCnt {
public:
    static const uint32 MAX_BUTTONS = 20;
    static const uint32 MAX_AXES = 10;

    BRK_API ~Joystick() override = default;

    /** @return Name of this device */
    const StringName &GetName() const { return mName; }

    /** @return Name guid this device */
    const StringName &GetGUID() const { return mGUID; }

    /** @return Current device state */
    InputDeviceState GetState() const { return mState; }

    /** @return Current states of device axes */
    const std::vector<float> &GetAxesStates() const { return mAxes; }

    /** @return Current states of device buttons */
    const std::vector<InputAction> &GetButtonsStates() const { return mButtons; }

protected:
    StringName mName;
    StringName mGUID;
    InputDeviceState mState;
    std::vector<float> mAxes;
    std::vector<InputAction> mButtons;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_INPUTDEVICES_HPP
