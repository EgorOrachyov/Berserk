/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLFWJOYSTICK_H
#define BERSERK_GLFWJOYSTICK_H

#include <Platform/InputDevices.h>
#include <Containers/TArrayStatic.h>
#include <GlfwSystem/GlfwDefinitions.h>

namespace Berserk {

    class GlfwJoystick : public Joystick {
    public:
        GlfwJoystick(int32 glfwID) {
            mGlfwID = glfwID;
            mName = glfwGetJoystickName(mGlfwID);
            mGuid = glfwGetJoystickGUID(mGlfwID);

            int32 axesCount;
            int32 buttonsCount;

            glfwGetJoystickAxes(mGlfwID, &axesCount);
            glfwGetJoystickButtons(mGlfwID, &buttonsCount);

            mAxes.resize(axesCount, 0.0f);
            mButtonsActions.resize(buttonsCount, EInputAction::Unknown);
            mButtonsStates.resize(buttonsCount, EInputAction::Release);
        }

        ~GlfwJoystick() override = default;

        uint32 getAxesCount() const override {
            return mAxes.size();
        }

        uint32 getButtonsCount() const override {
            return mButtonsActions.size();
        }

        const CString &getName() const override {
            return mName;
        }

        const CString &getGuid() const override {
            return mGuid;
        }

        float getAxisValue(EJoystickAxis axis) const override {
            auto index = (uint32)axis;
            return index < mAxes.size()? mAxes[index] : 0.0f;
        }

        bool hasButtonInput() const override {
            return mHasButtonInput;
        }

        bool isConnected() const override {
            return mState == EDeviceState::Connected;
        }

        EInputAction getButtonAction(EJoystickButton button) const override {
            auto index = (uint32)button;
            return index < mButtonsActions.size()? mButtonsActions[index] : EInputAction::Unknown;
        }

        EDeviceState getState() const override {
            return mState;
        }

        void processConnect() {
            mState = EDeviceState::Connected;
        }

        void processDisconnect() {
            mState = EDeviceState::Disconnected;
        }

        void updateState() {
            mHasButtonInput = false;

            int32 axesCount;
            int32 buttonsCount;

            auto pAxes = glfwGetJoystickAxes(mGlfwID, &axesCount);

            for (int32 i = 0; i < axesCount; i++) {
                mAxes[i] = pAxes[i];
            }

            auto pButtons = glfwGetJoystickButtons(mGlfwID, &buttonsCount);

            // Glfw gives continuous states of buttons and pollutes the action events
            // Here we only care about state changes
            // action is (pressed|repeated) & state is released => button is released
            // action is released & state is pressed => button is pressed
            // action is (pressed|repeated) & state is pressed => button is repeated
            for (int32 i = 0; i < buttonsCount; i++) {
                EInputAction prev = mButtonsActions[i];
                EInputAction current = GlfwDefinitions::getAction(pButtons[i]);
                EInputAction action = EInputAction::Unknown;

                if ((prev == EInputAction::Press || prev == EInputAction::Repeat) && current == EInputAction::Release) {
                    action = EInputAction::Release;
                    mHasButtonInput = true;
                }

                if (prev == EInputAction::Release && current == EInputAction::Press) {
                    action = EInputAction::Press;
                    mHasButtonInput = true;
                }

                if ((prev == EInputAction::Press || prev == EInputAction::Repeat) && current == EInputAction::Press) {
                    action = EInputAction::Repeat;
                    mHasButtonInput = true;
                }

                mButtonsActions[i] = action;
                mButtonsStates[i] = current;
            }
        }

        void resetState() {
            mHasButtonInput = false;

            for (auto& button: mButtonsActions) {
                button = EInputAction::Unknown;
            }

            for (auto& button: mButtonsStates) {
                button = EInputAction::Release;
            }
        }

        int32 getGlfwID() const {
            return mGlfwID;
        }

    protected:

        friend class GlfwInput;

        CString mName;
        CString mGuid;
        int32 mGlfwID = -1;
        bool mHasButtonInput = false;
        EDeviceState mState = EDeviceState::Connected;
        TArrayStatic<float,MAX_AXES_COUNT> mAxes;
        TArrayStatic<EInputAction,MAX_BUTTONS_COUNT> mButtonsActions;
        TArrayStatic<EInputAction,MAX_BUTTONS_COUNT> mButtonsStates;

    };

}



#endif //BERSERK_GLFWJOYSTICK_H
