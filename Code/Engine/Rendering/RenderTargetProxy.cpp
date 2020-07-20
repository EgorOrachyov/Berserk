/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/RenderTargetProxy.h>
#include <LogMacro.h>

namespace Berserk {
    namespace Rendering {

        RenderTargetProxy::RenderTargetProxy(TPtrShared<RenderTarget> target, float posScale, float sizeScale) {
            BERSERK_COND_ERROR_RET(target.isNotNull(), "Target must be not null for target proxy");
            mRootTarget = std::move(target);
            mResizePolicy = ERenderTargetResizePolicy::AreaProportional;

            posScale = Math::max(0.0f, posScale);
            sizeScale = Math::max(0.0f, sizeScale);
            if (posScale + sizeScale > 1.0f) sizeScale = 1.0f - posScale;

            mProportionPos = Vec2f(posScale, posScale);
            mProportionSize = Vec2f(sizeScale, sizeScale);

            mTargetSize = mRootTarget->getTargetSize();
            mPixelFormat = mRootTarget->getTargetPixelFormat();
            mClearColor = mRootTarget->getClearColor();
            mDepthClearValue = mRootTarget->getDepthClearValue();
            mStencilClearValue = mRootTarget->getStencilClearValue();

            updateSize(Region2i(), mRootTarget->getRenderingArea());

            mRootTarget->addResizeListener(*this);
        }

        RenderTargetProxy::RenderTargetProxy(TPtrShared<RenderTarget> target, Size2i relativePos, Size2i size) {
            BERSERK_COND_ERROR_RET(target.isNotNull(), "Target must be not null for target proxy");
            mRootTarget = std::move(target);
            mResizePolicy = ERenderTargetResizePolicy::AreaFit;

            auto width = Math::max(size[0], 0);
            auto height = Math::max(size[1], 0);

            mRenderingArea.getX() = mRootTarget->getRenderingArea().getX() + relativePos[0];
            mRenderingArea.getY() = mRootTarget->getRenderingArea().getY() + relativePos[1];
            mRenderingArea.getW() = width;
            mRenderingArea.getH() = height;

            mTargetSize = mRootTarget->getTargetSize();
            mPixelFormat = mRootTarget->getTargetPixelFormat();
            mClearColor = mRootTarget->getClearColor();
            mDepthClearValue = mRootTarget->getDepthClearValue();
            mStencilClearValue = mRootTarget->getStencilClearValue();

            updateSize(Region2i(), mRootTarget->getRenderingArea());

            mRootTarget->addResizeListener(*this);
        }

        RenderTargetProxy::~RenderTargetProxy() {
            mRootTarget->removeResizeListener(*this);
        }

        void RenderTargetProxy::onResized(const Region2i &oldArea, const Region2i &newArea) {
            mTargetSize = mRootTarget->getTargetSize();

            Region2i oldRenderingArea = mRenderingArea;
            updateSize(oldArea, newArea);
            Region2i newRenderingArea = mRenderingArea;

            BERSERK_LOG_INFO("Resize proxy render target Format::%s Pos::(%i,%i) Size::(%i,%i)",
                             EPixelFormatUtil::pixelFormatToString(mPixelFormat),
                             mRenderingArea[0], mRenderingArea[1], mRenderingArea[2], mRenderingArea[3]);

            for (auto listener: mResizeListeners) {
                listener->onResized(oldRenderingArea, newRenderingArea);
            }
        }

        void RenderTargetProxy::extractDeclaration(class RHIGraphicsPipelineDesc &desc) const {
            mRootTarget->extractDeclaration(desc);
        }

        void RenderTargetProxy::bind(RHIDrawList &drawList) const {

        }

        bool RenderTargetProxy::isScreenTarget() const {
            return mRootTarget->isScreenTarget();
        }

        bool RenderTargetProxy::isTextureTarget() const {
            return mRootTarget->isTextureTarget();
        }

        bool RenderTargetProxy::isProxyTarget() const {
            return true;
        }

        void RenderTargetProxy::updateSize(const Region2i &oldArea, const Region2i &newArea) {
            if (mResizePolicy == ERenderTargetResizePolicy::AreaFit) {
                auto x = mRenderingArea.getX();
                auto y = mRenderingArea.getY();
                auto w = mRenderingArea.getW();
                auto h = mRenderingArea.getH();

                mRenderingArea.getX() = Math::max(x, newArea.getX());
                mRenderingArea.getY() = Math::max(y, newArea.getY());
                mRenderingArea.getW() = Math::min(Math::max(newArea.getW() - mRenderingArea.getX(), 0), w);
                mRenderingArea.getH() = Math::min(Math::max(newArea.getH() - mRenderingArea.getY(), 0), h);
            }

            if (mResizePolicy == ERenderTargetResizePolicy::AreaProportional) {
                auto offsetX = mProportionPos[0] * (float) newArea.getW();
                auto offsetY = mProportionPos[1] * (float) newArea.getH();

                mRenderingArea.getX() = (int32) ((float) newArea.getX() + offsetX);
                mRenderingArea.getY() = (int32) ((float) newArea.getY() + offsetY);
                mRenderingArea.getW() = (int32) (mProportionSize[0] * (float) newArea.getW());
                mRenderingArea.getH() = (int32) (mProportionSize[1] * (float) newArea.getH());
            }
        }

    }
}