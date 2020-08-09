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
            if (event.inputAction == EInputAction::Press && event.keyboardKey == EKeyboardKey::Backspace) {
                if (mText.size() > 0) {
                    int32 last = (int32) mText.size() - 1;
                    mText.remove(last);
                }
            }
            else if (event.inputAction == EInputAction::Text) {
                mText.add(event.codepoint);
            }
        }

        return false;
    }

    void TextInput::setActive(bool enable) {
        mIsActive = enable;
    }

    WString TextInput::getTextAsString() const {
        return WString(mText.data(), mText.size());
    }
}