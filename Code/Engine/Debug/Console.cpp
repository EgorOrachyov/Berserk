/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Debug/Console.h>
#include <ConsoleRenderer.h>
#include <RenderModule.h>
#include <LogMacro.h>

namespace Berserk {

    Console* Console::gConsole = nullptr;

    Console::Console() {
        BERSERK_COND_ERROR_RET(gConsole == nullptr, "[BERSERK Engine] Allowed only single Console instance")
        gConsole = this;
        mManager = &ConsoleManager::getSingleton();

        auto& input = Input::getSingleton();
        input.addKeyboardListener(*this);

        auto& updateMan = UpdateManager::getSingleton();
        updateMan.subscribe(*this, EUpdateStage::PreUpdate);

        auto& renderer = Render::RenderModule::getSingleton();
        mRenderer = TPtrUnique<Render::ConsoleRenderer>::make(renderer.getPrimaryGraphics());

        BERSERK_LOG_INFO("Initialize In-Game Console");
    }

    Console::~Console() {
        mRenderer.free();

        auto& updateMan = UpdateManager::getSingleton();
        updateMan.unsubscribe(*this, EUpdateStage::PreUpdate);

        auto& input = Input::getSingleton();
        input.removeKeyboardListener(*this);

        gConsole = nullptr;

        BERSERK_LOG_INFO("Finalize In-Game Console");
    }

    bool Console::onKeyboardEvent(const InputEventKeyboard &event) {
        if (event.inputAction != EInputAction::Press) {
            return false;
        }

        bool bOpenFull = event.keyboardKey == mKeyOpenFull && event.modifiersMask.getFlag(mModOpenFull);
        if (bOpenFull) {
            openFull();
            return false;
        }

        bool bOpenPart = !mIsOpened && event.keyboardKey == mKeyOpenPart;
        if (bOpenPart) {
            openPart();
            return false;
        }

        bool bClose = mIsOpened && event.keyboardKey == mKeyClose;
        if (bClose) {
            close();
            return false;
        }

        if (!mIsOpened) {
            return false;
        }

        bool bCompletion = event.keyboardKey == mKeyCompletion;
        if (bCompletion) {
            mShowingCompletion = true;
            WString text = mTextInput.getTextAsString();
            CString ctext = std::move(CString::from(text));

            mCompletionListing.clear();
            mCompletionTypes.clear();

            static Function<void(const ConsoleObject&)> visitor = [&](const ConsoleObject& obj){
                WString name = std::move(WString::from(obj.getName()));

                mCompletionListing.move(name);
                mCompletionTypes.add(EOutputType::Text);
            };

            mManager->forEachConsoleObjectWithPrefix(ctext.data(), visitor);
            mRenderer->setEntries(mCompletionListing,mCompletionTypes);

            return false;
        }

        bool bEnterInput = event.keyboardKey == mKeyEnter && !mTextInput.isEmpty();
        if (bEnterInput) {
            mShowingCompletion = false;
            mCompletionOffset = mCompletionReset;

            WString text = mTextInput.getTextAsString();
            CString ctext = std::move(CString::from(text));

            mListing.move(text);
            mListingTypes.add(EOutputType::Input);

            mManager->processUserInput(ctext, *this);

            mRenderer->setEntries(mListing,mListingTypes);
            mRenderer->setInputText(WString());
            mTextInput.clearText();

            return false;
        }

        bool bHideCompletion = mShowingCompletion && event.keyboardKey == mKeyHideCompletion;
        if (bHideCompletion) {
            resetCompletion();
            return false;
        }

        bool bScrollUp = event.keyboardKey == mKeyScrollUp;
        if (bScrollUp && mShowingCompletion) {
            if (mCompletionListing.isEmpty()) {
                return false;
            }

            mCompletionOffset += 1;
            mCompletionOffset = Math::min(mCompletionOffset, (int32)mCompletionListing.size() - 1);

            mTextInput.setText(mCompletionListing[(int32)mCompletionListing.size() - 1 - mCompletionOffset]);
            return false;
        }

        if (bScrollUp) {
            mRenderer->scrollUp();
            return false;
        }

        bool bScrollDown = event.keyboardKey == mKeyScrollDown;
        if (bScrollDown && mShowingCompletion) {
            if (mCompletionListing.isEmpty()) {
                return false;
            }

            mCompletionOffset -= 1;

            if (mCompletionOffset < 0) {
                mCompletionOffset = mCompletionReset;
                mTextInput.clearText();
                return false;
            }

            mTextInput.setText(mCompletionListing[(int32)mCompletionListing.size() - 1 - mCompletionOffset]);
            return false;
        }

        if (bScrollDown) {
            mRenderer->scrollDown();
            return false;
        }

        return false;
    }

    void Console::openPart() {
        mIsOpened = true;
        mTextInput.setActive(true);
        mRenderer->openPart();
    }

    void Console::openFull() {
        mIsOpened = true;
        mTextInput.setActive(true);
        mRenderer->openFull();
    }

    void Console::close() {
        resetCompletion();
        mIsOpened = false;
        mTextInput.setActive(false);
        mRenderer->closeFull();
    }

    void Console::resetCompletion() {
        if (!mShowingCompletion)
            return;

        mCompletionOffset = mCompletionReset;
        mShowingCompletion = false;
        mRenderer->setEntries(mListing, mListingTypes);
    }

    void Console::onStageExec(Berserk::EUpdateStage stage) {
        if (mIsOpened) {
            mRenderer->setInputText(std::move(mTextInput.getTextAsString()));
        }
    }

    void Console::print(Berserk::EOutputType messageType, const char *message) {
        // Process multi-line text

        const char* start = message;
        uint32 position = 0;
        uint32 len = 0;

        while (message[position] != '\0') {
            if (message[position] == '\n') {
                WString text = std::move(WString::from(CString(start, len)));
                mListing.move(text);
                mListingTypes.add(messageType);

                start = &message[position + 1];
                len = 0;
            }

            position += 1;
            len += 1;
        }

        if (len > 0) {
            WString text = std::move(WString::from(CString(start,len)));
            mListing.move(text);
            mListingTypes.add(messageType);
        }
    }

    Console& Console::getSingleton() {
        return *gConsole;
    }

}