/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Input/TextInput.h>

namespace Berserk {

    TextInput::TextInput() {
        auto& input = Input::getSingleton();
        input.addKeyboardListener(*this);
    }

    TextInput::~TextInput() {
        auto& input = Input::getSingleton();
        input.removeKeyboardListener(*this);
    }

    bool TextInput::onKeyboardEvent(const InputEventKeyboard &event) {
        if (mIsActive) {
            if (event.keyboardKey == EKeyboardKey::Backspace && event.pressed()) {
                if (mText.size() > 0) {
                    int32 last = (int32) mText.size() - 1;
                    mText.remove(last);
                }
            }
            else if (event.text()) {
                mText.add(event.codepoint);
            }
        }

        return false;
    }

    void TextInput::setActive(bool enable) {
        mIsActive = enable;
    }

    void TextInput::setText(const Berserk::WString &text) {
        mText.clear();

        for (uint32 i = 0; i < text.length(); i++) {
            mText.add(text.data()[i]);
        }
    }

    WString TextInput::getTextAsString() const {
        return WString(mText.data(), mText.size());
    }
}