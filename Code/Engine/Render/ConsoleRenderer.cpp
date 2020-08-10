/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ConsoleRenderer.h>
#include <Paths.h>
#include <Engine.h>

namespace Berserk {
    namespace Render {

        ConsoleRenderer::ConsoleRenderer(TPtrShared<Render::Graphics> graphics) {
            BERSERK_COND_ERROR_RET(graphics.isNotNull(), "Passed null graphics");
            mGraphics = std::move(graphics);

            auto fontInput = Paths::getFullPathFor(mFontPathInput, EPathType::Engine);
            mFontInput = TPtrShared<GpuFont>::make(mFontHeight, fontInput);

            auto fontBack = Paths::getFullPathFor(mFontPathListing, EPathType::Engine);
            mFontListing = TPtrShared<GpuFont>::make(mFontListingHeight, fontBack);

            UpdateManager::getSingleton().subscribe(*this, EUpdateStage::PreUpdate);
        }

        ConsoleRenderer::~ConsoleRenderer() {
            UpdateManager::getSingleton().unsubscribeFromAll(*this);
        }

        void ConsoleRenderer::setInputText(WString text) {
            mTextInput = std::move(text);
            mCursorPos = mTextBaseOffset + mFontInput->getTextSize(mTextInput)[0];
        }

        void ConsoleRenderer::addEntry(WString entry, EOutputType type) {
            mTextListing.move(entry);
            mTextListingTypes.move(type);
        }

        void ConsoleRenderer::addEntries(const TArray<WString> &entries, const TArray<EOutputType> &types) {
            BERSERK_COND_ERROR_RET(entries.size() == types.size(), "Size of the entries and types must be the same");
            mTextListing.add(entries);
            mTextListingTypes.add(types);
        }
        
        void ConsoleRenderer::setEntries(const TArray<WString> &entries, const TArray<EOutputType> &types) {
            BERSERK_COND_ERROR_RET(entries.size() == types.size(), "Size of the entries and types must be the same");
            mTextListing = entries;
            mTextListingTypes = types;
            mListingScrollOffset = 0;
        }

        void ConsoleRenderer::clearEntries() {
            mTextListing.clear();
            mTextListingTypes.clear();
            mListingScrollOffset = 0;
        }

        void ConsoleRenderer::openPart() {
            if (mStateCurrent == EConsoleState::OpenedPart)
                return;

            mStateCurrent = EConsoleState::Opening;
            mStateTarget = EConsoleState::OpenedPart;

            auto h = (int32)(mHeightPart * (float)mGraphics->getSize()[1]);
            auto H = (int32)(mHeightFull * (float)mGraphics->getSize()[1]);
            auto s = (int32)(mStepToOpenPart * (float)H);

            mHeightTarget = mPosition[1] + h;
            mDeltaStep = s * (mHeightTarget > mHeightCurrent? 1: -1);
        }

        void ConsoleRenderer::openFull() {
            if (mStateCurrent == EConsoleState::Opened)
                return;

            mStateCurrent = EConsoleState::Opening;
            mStateTarget = EConsoleState::Opened;

            auto h = (int32)(mHeightFull * (float)mGraphics->getSize()[1]);
            auto s = (int32)(mStepToOpenFull * (float)h);

            mHeightTarget = mPosition[1] + h;
            mDeltaStep = s;
        }

        void ConsoleRenderer::closeFull() {
            if (mStateCurrent == EConsoleState::Closed)
                return;

            mStateCurrent = EConsoleState::Closing;
            mStateTarget = EConsoleState::Closed;

            auto h = (int32)(mHeightFull * (float)mGraphics->getSize()[1]);
            auto s = (int32)(mStepToOpenFull * (float)h);

            mHeightTarget = mPosition[1];
            mDeltaStep = -s;
        }

        void ConsoleRenderer::resetScroll() {
            mListingScrollOffset = 0;
        }

        void ConsoleRenderer::scrollUp() {
            mListingScrollOffset += 1;
        }

        void ConsoleRenderer::scrollDown() {
            mListingScrollOffset -= 1;
            mListingScrollOffset = (mListingScrollOffset < 0? 0: mListingScrollOffset);
        }

        void ConsoleRenderer::focusOnInput() {
            mCurrentTimeFocus = 0;
            mDrawFocus = true;
        }

        void ConsoleRenderer::update() {
            float dt = Engine::getSingleton().getFrameTime();

            // Cursor blink update
            {
                mCurrentTimeBlink += dt;
                if (mCurrentTimeBlink >= mTimeBlink) {
                    mCurrentTimeBlink = 0;
                    mCurrentBlinkVisible = !mCurrentBlinkVisible;
                }
            }

            // Focus on input area
            {
                mCurrentTimeFocus += dt;
                if (mCurrentTimeFocus >= mTimeFocus) {
                    mCurrentTimeFocus = 0;
                    mDrawFocus = false;
                }
            }

            // Text scrolling update
            {
                int32 y = mHeightCurrent - mTextInputHeight - mTextListingBaseLine;
                int32 h = y - mPosition.y();
                int32 canShow = h / (mFontListingHeight + mTextListingStep);
                int32 willShow = Math::min(canShow, (int32) mTextListing.size());

                mListingScrollOffset = Math::min(mListingScrollOffset, (int32)mTextListing.size() - willShow);
            }

            // Smooth animated console opening/closing update
            switch (mStateCurrent) {
                case EConsoleState::Opening: {
                    mHeightCurrent += (int32)((float)mDeltaStep * dt);

                    auto movementDown = (mDeltaStep > 0);
                    if (movementDown) {
                        if (mHeightCurrent >= mHeightTarget) {
                            mHeightCurrent = mHeightTarget;
                            mStateCurrent = mStateTarget;
                        }
                    }
                    else {
                        if (mHeightCurrent <= mHeightTarget) {
                            mHeightCurrent = mHeightTarget;
                            mStateCurrent = mStateTarget;
                        }
                    }
                }
                break;

                case EConsoleState::Closing: {
                    mHeightCurrent += (int32)((float)mDeltaStep * dt);

                    if (mHeightCurrent <= mHeightTarget) {
                        mHeightCurrent = mHeightTarget;
                        mStateCurrent = mStateTarget;
                    }
                }
                break;

                default:
                    return;
            }
        }

        void ConsoleRenderer::draw() {
            if (mStateCurrent == EConsoleState::Closed)
                return;

            auto& p = mPen;
            auto& g = *mGraphics;

            auto size = g.getSize();

            int32 y = 0;
            int32 x = 0;
            int32 w = 0;
            int32 h = 0;

            p.setUsingAlpha(false);

            h = mTextInputHeight;
            w = size.width();
            x = mPosition.x();
            y = mHeightCurrent-h;
            if (mDrawFocus) {
                float half = mTimeFocus * 0.5f;
                float factor = mCurrentTimeFocus >= half ? (mTimeFocus - mCurrentTimeFocus) / half: mCurrentTimeFocus / half;
                p.setColor(Color4f::lerp(factor, mColorFocusC1, mColorFocusC2));
            }
            else {
                p.setColor(mColorInput);
            }
            g.drawFilledRect(p, {x,y}, {w,h});

            y = mHeightCurrent - mTextBaseLine;
            x = mTextBaseOffset;
            p.setColor(mColorInputText);
            g.drawText(p, {x,y}, mTextInput, mFontInput, mFontHeight);

            if (mCurrentBlinkVisible) {
                x = mPosition.x() + mCursorPos;
                y = mHeightCurrent - mCursorBase - mCursorHeight;
                w = mCursorWidth;
                h = mCursorHeight;
                p.setColor(mColorCursor);
                g.drawFilledRect(p, {x,y}, {w,h});
            }

            if (mHeightCurrent - mTextInputHeight >= mPosition.y()) {
                w = size.width();
                h = mHeightCurrent - mTextInputHeight - mPosition.y();
                x = mPosition.x();
                y = mPosition.y();
                p.setColor(mColorListing);
                g.drawFilledRect(p, {x,y}, {w,h});

                x = mPosition.x() + mTextBaseOffset;
                y = mHeightCurrent - mTextInputHeight - mTextListingBaseLine;

                for (int32 i = (int32) mTextListing.size() - 1 - mListingScrollOffset; i >= 0; i--) {
                    if (y < mPosition.y())
                        break;

                    auto t = mTextListingTypes[i];

                    switch (t) {
                        case EOutputType::Error:
                            p.setColor(mColorListingError);
                            break;
                        case EOutputType::Warning:
                            p.setColor(mColorListingWarning);
                            break;
                        case EOutputType::Info:
                            p.setColor(mColorListingInfo);
                            break;
                        case EOutputType::Input:
                            p.setColor(mColorListingInput);
                            break;
                        default:
                            p.setColor(mColorListingText);
                            break;
                    }

                    g.drawText(p, {x, y}, mTextListing[i], mFontListing);

                    y -= mTextListingStep + mFontListingHeight;
                }
            }
        }

    }
}