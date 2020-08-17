/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_INPUT_H
#define BERSERK_INPUT_H

#include <Platform/InputEvents.h>
#include <Platform/InputDevices.h>
#include <Platform/InputDefinitions.h>

namespace Berserk {

    /** Game Thread only mouse input listener */
    class InputListenerMouse {
    public:
        /**
         * Called every time mouse input event generated
         * @param event Mouse event data
         * @return True if broadcasting of this event must be stopped (no other listener will hear it)
         */
        virtual bool onMouseEvent(const InputEventMouse& event) = 0;
    };

    /** Game Thread only keyboard input listener */
    class InputListenerKeyboard {
    public:
        /**
         * Called every time keyboard input event generated
         * @param event Keyboard event data
         * @return True if broadcasting of this event must be stopped (no other listener will hear it)
         */
        virtual bool onKeyboardEvent(const InputEventKeyboard& event) = 0;
    };

    /** Game Thread only joystick input listener */
    class InputListenerJoystick {
    public:
        /**
         * Called every time joystick input event generated
         * @param event Joystick event data
         * @return True if broadcasting of this event must be stopped (no other listener will hear it)
         */
        virtual bool onJoystickEvent(const InputEventJoystick& event) = 0;
    };

    /** Game Thread only drop input listener */
    class InputListenerDrop {
    public:
        /**
         * Called every time drop input event generated
         * @param event Drop event data
         * @return True if broadcasting of this event must be stopped (no other listener will hear it)
         */
        virtual bool onDropInput(const InputEventDrop& event) = 0;
    };

    /**
     * @brief Platform input
     *
     * Handles raw OS input from various devices and allows
     * to subscribe to devices input and listen to various input events.
     *
     * @note Could be accessed only from main thread
     * @note Process input and notify listeners in the out of the main update section time.
     */
    class Input {
    public:
        Input();
        virtual ~Input() = default;

        /** Adds input event listener */
        virtual void addMouseListener(InputListenerMouse& listener) = 0;

        /** Removes input event listener */
        virtual void removeMouseListener(InputListenerMouse& listener) = 0;

        /** Adds input event listener */
        virtual void addKeyboardListener(InputListenerKeyboard& listener) = 0;

        /** Removes input event listener */
        virtual void removeKeyboardListener(InputListenerKeyboard& listener) = 0;

        /** Adds input event listener */
        virtual void addJoystickListener(InputListenerJoystick& listener) = 0;

        /** Removes input event listener */
        virtual void removeJoystickListener(InputListenerJoystick& listener) = 0;

        /** Adds input event listener */
        virtual void addDropListener(InputListenerDrop& listener) = 0;

        /** Removes input event listener */
        virtual void removeDropListener(InputListenerDrop& listener) = 0;

        /** @return Modifiers state */
        virtual ModifiersMask getModifiersMask() const = 0;

        virtual Point2i getMousePosition() const = 0;
        virtual Point2i getMouseDelta() const = 0;
        virtual bool isMouseMoved() const = 0;
        virtual bool isButtonPressed(EMouseButton button) const = 0;
        virtual bool isButtonReleased(EMouseButton button) const = 0;

        virtual bool isKeyPressed(EKeyboardKey key) const = 0;
        virtual bool isKeyReleased(EKeyboardKey key) const = 0;
        virtual bool isKeyRepeated(EKeyboardKey key) const = 0;

        /** @return True if has drop input since last input update cycle */
        virtual bool hasDropInput() const = 0;

        /** Query drop input strings */
        virtual void getDropInput(TArray<CString> &drop) const = 0;

        /** @return Mouse device if present */
        virtual TPtrShared<Mouse> getMouse() const  = 0;

        /** @return Keyboard device if present */
        virtual TPtrShared<Keyboard> getKeyboard() const  = 0;

        /** @return True if has joysticks */
        virtual bool hasJoysticks() const = 0;

        /** @return True if has connected joysticks */
        virtual bool hasConnectedJoysticks() const = 0;

        /** @return Count of joysticks */
        virtual uint32 getJoysticksCount() const = 0;

        /** Query all the joystick devices */
        virtual void getJoysticks(TArray<TPtrShared<Joystick>> &joysticks) = 0;

        /** @return Singleton input instance */
        static Input& getSingleton();

    private:

        /** Singleton reference */
        static Input* gInput;
    };

}

#endif //BERSERK_INPUT_H