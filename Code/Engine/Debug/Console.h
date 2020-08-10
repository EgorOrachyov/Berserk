/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CONSOLE_H
#define BERSERK_CONSOLE_H

#include <Input/TextInput.h>
#include <TPtrUnique.h>
#include <RenderModuleFwd.h>
#include <UpdateManager.h>
#include <IO/Log.h>
#include <IO/OutputDevice.h>

namespace Berserk {

    /**
     * @brief In-game developer console
     *
     * In-game developer console with basic text input, listing scroll,
     * log messages collector, texts coloring and console variables/commands access.
     *
     * Provides access to console variables, which could be queried and modified
     * through this console, provides basic text completion for the input, allows
     * to run console commands with string params.
     *
     * Architecture notes:
     * ConsoleManager <--- Console ---> ConsoleRenderer
     *    (model)        (controller)       (view)
     *
     * User interface:
     *  - Keyboard keys to text typing
     *  - ENTER to submit command
     *  - SHIFT + UP / SHIFT + DOWN to scroll console content
     *  - TAB to show completion variants
     *  - UP / DOWN to navigate user input history
     *  - UP / DOWN to navigate completion variants (if previously was TAB pressed to show completion variants)
     *
     * @note Main-thread only
     * @note Uses ConsoleRenderer for rendering on GPU side (in render thread)
     */
    class Console final: public InputListenerKeyboard, public UpdateStageListener, public OutputDevice {
    public:

        Console();
        ~Console();

        /** Explicitly open part of the console (compact mode) */
        void openPart();

        /** Explicitly open the console (full mode) */
        void openFull();

        /** Explicitly close console */
        void close();

        /** Reset scroll state */
        void resetScroll();

        /** Reset completion state */
        void resetCompletion();

        /** @return True if console is opened */
        bool isOpened() const { return mIsOpened; }

        /** Input event listener */
        bool onKeyboardEvent(const InputEventKeyboard &event) override;

        /** Update console state for each frame */
        void onStageExec(EUpdateStage stage) override;

        /** Pass to the ConsoleManager to be able to capture manager exec info */
        void print(EOutputType messageType, const char *message) override;

        /** @return Engine singleton console instance */
        static Console& getSingleton();

    private:

        /** Input settings */
        EModifier mModOpenFull = EModifier::Shift;
        EModifier mModScroll = EModifier::Shift;
        EKeyboardKey mKeyOpenPart = EKeyboardKey::F1;
        EKeyboardKey mKeyOpenFull = EKeyboardKey::F1;
        EKeyboardKey mKeyClose = EKeyboardKey::F1;
        EKeyboardKey mKeyCompletion = EKeyboardKey::Tab;
        EKeyboardKey mKeyEnter = EKeyboardKey::Enter;
        EKeyboardKey mKeyHideCompletion = EKeyboardKey::Escape;
        EKeyboardKey mKeyScrollUp = EKeyboardKey::Up;
        EKeyboardKey mKeyScrollDown = EKeyboardKey::Down;

        /** User input history */
        TArray<WString> mUserInput;

        /** Listing shown to the user */
        TArray<WString> mListing;
        TArray<EOutputType> mListingTypes;

        /** Completion listing shown to the user when he presses mKeyCompletion */
        TArray<WString> mCompletionListing;
        TArray<EOutputType> mCompletionTypes;

        /** Handles keyboard text input */
        TextInput mTextInput;

        /** Cached reference to manager */
        class ConsoleManager* mManager = nullptr;

        /** Cached reference to log messages */
        class ConsoleMessages* mMessages = nullptr;

        /** True if console currently open (otherwise closed) */
        bool mIsOpened = false;

        /** True if showing completion entries */
        bool mShowingCompletion = false;

        /** Position of the scroll (as offset from last element) */
        int32 mScrollReset = -1;
        int32 mScrollOffset = mScrollReset;

        /** Rendering state on GPU side */
        TPtrUnique<class Render::ConsoleRenderer> mRenderer;

        /** Globally accessible instance */
        static Console* gConsole;

    };

}

#endif //BERSERK_CONSOLE_H