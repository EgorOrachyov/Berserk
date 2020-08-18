/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLFWKEYBOARD_H
#define BERSERK_GLFWKEYBOARD_H

#include <Platform/InputDevices.h>

namespace Berserk {

    class GlfwKeyboard final: public Keyboard {
    public:
        GlfwKeyboard() {
            for (auto& key: mKeys) {
                key = EInputAction::Unknown;
            }
        }

        ~GlfwKeyboard() override = default;

        const CString &getName() const override {
            return mName;
        }

        void getTextInput(TArray<wchar> &codePoints) const override {
            codePoints = mCodePoints;
        }

        bool hasTextInput() const override {
            return mHasTextInput;
        }

        EInputAction getKeyAction(EKeyboardKey key) const override {
            auto index = (uint32)key;
            return index < MAX_KEYS_COUNT ? mKeys[index] : EInputAction::Unknown;
        }

        bool hasKeyInput() const override {
            return mHasKeyInput;
        }

        ModifiersMask getModifiersMask() const override {
            return mModifiersMask;
        }

        EDeviceState getState() const override {
            return mState;
        }

        void processTextInput(wchar codePoint) {
            mCodePoints.add(codePoint);
            mHasTextInput = true;
        }

        void processKeyAction(EKeyboardKey key, EInputAction action, ModifiersMask mask) {
            auto index = (uint32)key;

            if (index < MAX_KEYS_COUNT) {
                mKeys[index] = action;
                mHasKeyInput = true;
                mModifiersMask |= mask;
            }
        }

        void processModifiersUpdate(ModifiersMask mask) {
            mModifiersMask |= mask;
        }

        void updateState() {
            mHasTextInput = false;
            mHasKeyInput = false;
            mCodePoints.clear();
            mModifiersMask = ModifiersMask();

            for (auto& key: mKeys) {
                // If key was pressed, then it goes to be repeated until it is released
                if (key == EInputAction::Press) {
                    key = EInputAction::Repeat;
                }

                // If repeated, then has some input on keys
                if (key == EInputAction::Repeat) {
                    mHasKeyInput = true;
                }
                else {
                    key = EInputAction::Unknown;
                }
            }
        }

        const TArray<wchar> &getGlfwCodePoints() const {
            return mCodePoints;
        }

    protected:

        friend class GlfwInput;

        CString mName = "GlfwKeyboardDevice";
        ModifiersMask mModifiersMask;
        bool mHasKeyInput = false;
        bool mHasTextInput = false;
        EDeviceState mState = EDeviceState::Connected;
        EInputAction mKeys[MAX_KEYS_COUNT] = {};
        TArray<wchar> mCodePoints;
    };

}

#endif //BERSERK_GLFWKEYBOARD_H