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

            ConsoleRenderer(TPtrShared<Graphics> graphics);
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
            GraphicsPen mPen;

            TPtrShared<GpuFont> mFontListing;
            TPtrShared<GpuFont> mFontInput;

            WString mTextInput;
            TArray<WString> mTextListing;
            TArray<EOutputType> mTextListingTypes;

            Color4f mColorListing = Color4f::fromHex(0x303030ff);
            Color4f mColorInput = Color4f::fromHex(0x404040ff);
            Color4f mColorListingInput = Color4f::fromHex(0x43fc3eff);
            Color4f mColorListingText = Color4f::fromHex(0xb5b5b5ff);
            Color4f mColorListingInfo = Color4f::fromHex(0xdededeff);
            Color4f mColorListingWarning = Color4f::fromHex(0xffea00ff);
            Color4f mColorListingError = Color4f::fromHex(0xff0000ff);
            Color4f mColorInputText = Color4f::fromHex(0xedededff);
            Color4f mColorCursor = Color4f::fromHex(0xf2f2f2ff);

            EConsoleState mStateCurrent = EConsoleState::Closed;
            EConsoleState mStateTarget = EConsoleState::Closed;

            CString mFontPathListing = "Assets/Fonts/Anonymous_Pro_B.ttf";
            CString mFontPathInput = "Assets/Fonts/Anonymous_Pro_B.ttf";

            Point2i mPosition;
            float mHeightFull = 0.6f;
            float mHeightPart = 0.2f;
            float mStepToOpenFull = 4.f;
            float mStepToOpenPart = 4.f;
            float mTimeBlink = 0.6f;

            int32 mTextInputHeight = 40;
            int32 mFontHeight = 32;
            int32 mFontListingHeight = 30;
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
            bool mCurrentBlinkVisible = false;
        };


    }
}




#endif //BERSERK_CONSOLERENDERER_H
