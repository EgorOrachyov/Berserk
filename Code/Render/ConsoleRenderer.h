/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CONSOLERENDERER_H
#define BERSERK_CONSOLERENDERER_H

#include <Graphics.h>
#include <UpdateManager.h>
#include <IO/OutputDevice.h>

namespace Berserk {
    namespace Render {

        // todo: render thread update

        /** Console renderer for rendering in-game developer console */
        class ConsoleRenderer : public UpdateStageListener {
        public:

            ConsoleRenderer(TPtrShared<Graphics> graphics, TPtrShared<GpuFont> font);
            ~ConsoleRenderer();

            void setInputText(WString text);
            void addEntry(WString entry, EOutputType type);
            void addEntries(const TArray<WString> &entries, const TArray<EOutputType> &types);
            void setEntries(const TArray<WString> &entries, const TArray<EOutputType> &types);
            void clearEntries();
            void openPart();
            void openFull();
            void closeFull();
            void resetScroll();
            void scrollUp();
            void scrollDown();
            void focusOnInput();

            void update();
            void draw();

            // todo: render thread update
            void onStageExec(EUpdateStage stage) override {
                update();
                draw();
            }

        private:

            enum class EConsoleState {
                Closed,
                Opening,
                Opened,
                OpenedPart,
                Closing
            };

            TPtrShared<Graphics> mGraphics;
            TPtrShared<GpuFont> mFont;
            GraphicsPen mPen;

            WString mTextInput;
            TArray<WString> mTextListing;
            TArray<EOutputType> mTextListingTypes;

            Color4f mColorListing = Color4f::fromHex(0x1a1a1abe);
            Color4f mColorInput = Color4f::fromHex(0x303030be);
            Color4f mColorListingInput = Color4f::fromHex(0x43fc3eff);
            Color4f mColorListingText = Color4f::fromHex(0xf2f2f2ff);
            Color4f mColorListingInfo = Color4f::fromHex(0xfbf0ffff);
            Color4f mColorListingWarning = Color4f::fromHex(0xffea00ff);
            Color4f mColorListingError = Color4f::fromHex(0xff0000ff);
            Color4f mColorInputText = Color4f::fromHex(0xf2f2f2ff);
            Color4f mColorCursor = Color4f::fromHex(0xf2f2f2be);
            Color4f mColorFocusC2 = Color4f::fromHex(0x606060be);
            Color4f mColorFocusC1 = Color4f::fromHex(0x303030be);

            EConsoleState mStateCurrent = EConsoleState::Closed;
            EConsoleState mStateTarget = EConsoleState::Closed;

            Point2i mPosition;
            float mHeightFull = 0.6f;
            float mHeightPart = 0.2f;
            float mStepToOpenFull = 4.f;
            float mStepToOpenPart = 4.f;
            float mTimeBlink = 0.6f;
            float mTimeFocus = 0.4f;

            int32 mTextInputHeight = 40;
            int32 mTextHeightInput = 32;
            int32 mTextHeightListing = 28;
            int32 mTextListingStep = 2;
            int32 mTextListingBaseLine = 10;
            int32 mTextBaseLine = 10;
            int32 mTextBaseOffset = 10;
            int32 mCursorWidth = 16;
            int32 mCursorHeight = 32;
            int32 mCursorBase = 4;

            int32 mDeltaStep = 0;
            int32 mHeightCurrent = 0;
            int32 mHeightTarget = 0;
            int32 mCursorPos = mTextBaseOffset;
            int32 mListingScrollOffset = 0;

            float mCurrentTimeBlink = 0;
            float mCurrentTimeFocus = 0;
            bool mCurrentBlinkVisible = false;
            bool mDrawFocus = false;
        };


    }
}




#endif //BERSERK_CONSOLERENDERER_H
