/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TEXTINPUT_H
#define BERSERK_TEXTINPUT_H

#include <Platform/Input.h>
#include <String/WString.h>

namespace Berserk {

    /**
     * @brief Text input
     *
     * Component for generic simple keyboard text writer.
     * @note Main-thread only
     */
    class TextInput : public InputListenerKeyboard {
    public:

        TextInput();
        ~TextInput();

        bool onKeyboardEvent(const InputEventKeyboard &event) override;

        /** Clear text content */
        void clearText() { mText.clear(); }

        /** Enable keyboard listening */
        void setActive(bool enable);

        /** Set text */
        void setText(const WString& text);

        /** @return Text input as string */
        WString getTextAsString() const;

        /** @return True if currently active */
        bool isActive() const { return mIsActive; }

        /** @return True if text is empty */
        bool isEmpty() const { return mText.isEmpty(); }

        /** @return Raw text array */
        const TArray<wchar> &getText() const { return mText; }

        /** @return Number of character in the text */
        uint32 getTextSize() const { return mText.size(); }

    private:

        /** Current text chars */
        TArray<wchar> mText;

        /** True if enabled and listens to new input chars */
        bool mIsActive = false;

    };

}

#endif //BERSERK_TEXTINPUT_H