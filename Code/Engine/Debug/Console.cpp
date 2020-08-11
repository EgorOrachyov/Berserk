/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Debug/Console.h>
#include <Debug/ConsoleMessages.h>
#include <ConsoleRenderer.h>
#include <RenderModule.h>
#include <LogMacro.h>
#include <Console/ConsoleManager.h>

namespace Berserk {

    Console* Console::gConsole = nullptr;

    Console::Console() {
        BERSERK_COND_ERROR_RET(gConsole == nullptr, "[BERSERK Engine] Allowed only single Console instance")
        gConsole = this;
        mManager = &ConsoleManager::getSingleton();
        mMessages = &ConsoleMessages::getSingleton();

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
        resetScroll();
        mIsOpened = false;
        mTextInput.setActive(false);
        mRenderer->closeFull();
    }

    void Console::resetScroll() {
        mScrollOffset = mScrollReset;
    }

    void Console::resetCompletion() {
        if (mShowingCompletion) {
            mShowingCompletion = false;
            mRenderer->setEntries(mListing, mListingTypes);
        }
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
            mRenderer->resetScroll();
            return false;
        }

        if (!mIsOpened) {
            return false;
        }

        bool bCompletion = event.keyboardKey == mKeyCompletion;
        if (bCompletion) {
            resetScroll();
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

        bool bEnterInput = event.keyboardKey == mKeyEnter;
        if (bEnterInput) {
            if (mTextInput.isEmpty()) {
                mRenderer->focusOnInput();
                return false;
            }

            resetScroll();
            mShowingCompletion = false;

            WString text = mTextInput.getTextAsString();
            CString ctext = std::move(CString::from(text));

            mUserInput.add(text);
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
            resetScroll();
            resetCompletion();
            return false;
        }

        bool bScrollUp = event.keyboardKey == mKeyScrollUp;
        if (bScrollUp && event.modifiersMask.getFlag(mModScroll)) {
            mRenderer->scrollUp();
            return false;
        }

        if (bScrollUp && mShowingCompletion) {
            if (mCompletionListing.isEmpty()) {
                mRenderer->focusOnInput();
                return false;
            }

            mScrollOffset += 1;
            mScrollOffset = Math::min(mScrollOffset, (int32)mCompletionListing.size() - 1);

            mTextInput.setText(mCompletionListing[(int32)mCompletionListing.size() - 1 - mScrollOffset]);
            return false;
        }

        if (bScrollUp) {
            if (mUserInput.isEmpty()) {
                mRenderer->focusOnInput();
                return false;
            }

            mScrollOffset += 1;
            mScrollOffset = Math::min(mScrollOffset, (int32)mUserInput.size() - 1);

            mTextInput.setText(mUserInput[(int32)mUserInput.size() - 1 - mScrollOffset]);
            return false;
        }

        bool bScrollDown = event.keyboardKey == mKeyScrollDown;
        if (bScrollDown && event.modifiersMask.getFlag(mModScroll)) {
            mRenderer->scrollDown();
            return false;
        }

        if (bScrollDown && mShowingCompletion) {
            if (mCompletionListing.isEmpty()) {
                mRenderer->focusOnInput();
                return false;
            }

            mScrollOffset -= 1;

            if (mScrollOffset < 0) {
                resetScroll();
                mTextInput.clearText();
                return false;
            }

            mTextInput.setText(mCompletionListing[(int32)mCompletionListing.size() - 1 - mScrollOffset]);
            return false;
        }

        if (bScrollDown) {
            if (mUserInput.isEmpty()) {
                mRenderer->focusOnInput();
                return false;
            }

            mScrollOffset -= 1;

            if (mScrollOffset < 0) {
                resetScroll();
                mTextInput.clearText();
                return false;
            }

            mTextInput.setText(mUserInput[(int32)mUserInput.size() - 1 - mScrollOffset]);
            return false;
        }

        return false;
    }

    void Console::onStageExec(Berserk::EUpdateStage stage) {
        if (mIsOpened) {
            mRenderer->setInputText(std::move(mTextInput.getTextAsString()));

            uint32 size = mListing.size();

            if (mMessages->getMessagesCount() > 0) {
                mMessages->popMessages(mListing, mListingTypes);

                if (mListing.size() > size) {
                    mRenderer->setEntries(mListing, mListingTypes);
                }
            }
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