/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLFWMOUSE_H
#define BERSERK_GLFWMOUSE_H

#include <Platform/InputDevices.h>

namespace Berserk {

    class GlfwMouse : public Mouse {
    public:
        GlfwMouse() {
            for (auto& button: mButtons) {
                button = EInputAction::Unknown;
            }
        }

        ~GlfwMouse() override = default;

        const CString &getName() const override {
            return mName;
        }

        EInputAction getButtonAction(EMouseButton button) const override {
            auto index = (uint32)button;
            return index < MAX_BUTTONS_COUNT ? mButtons[index] : EInputAction::Unknown;
        }

        Point2i getPosition() const override {
            return mPosition;
        }

        Point2i getPositionDelta() const override {
            return mDelta;
        }

        bool isMoved() const override {
            return mIsMoved;
        }

        bool hasButtonInput() const override {
            return mHasButtonInput;
        }

        EDeviceState getState() const override {
            return mState;
        }

        void processMovement(const Point2i& newPosition) {
            mDelta = newPosition - mPosition;
            mPosition = newPosition;
            mIsMoved = true;
        }

        void processButtonClick(EMouseButton button, EInputAction action) {
            auto index = (uint32)button;

            if (index < MAX_BUTTONS_COUNT) {
                mButtons[index] = action;
                mHasButtonInput = true;
            }
        }

        void updateState() {
            mIsMoved = false;
            mHasButtonInput = false;
            mDelta = Point2i();

            for (auto& button: mButtons) {
                button = EInputAction::Unknown;
            }
        }

    protected:

        friend class GlfwInput;

        CString mName = "GlfwMouseDevice";
        Point2i mPosition;
        Point2i mDelta;
        bool mIsMoved = false;
        bool mHasButtonInput = false;
        EDeviceState mState = EDeviceState::Connected;
        EInputAction mButtons[MAX_BUTTONS_COUNT] = {};

    };

}

#endif //BERSERK_GLFWMOUSE_H