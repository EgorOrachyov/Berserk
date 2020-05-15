/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_IRENDERTARGET_H
#define BERSERK_IRENDERTARGET_H

#include <Typedefs.h>
#include <PixelFormat.h>
#include <Math/Color4.h>
#include <Math/Point2i.h>
#include <Math/Region2i.h>
#include <RHI/RHIResources.h>

namespace Berserk {
    namespace Rendering {

        /** Listener for cascade update of render target resize event */
        class IRenderTargetResizeListener {
        public:
            /**
             * Called when render target rendering area resized
             * @param oldArea Previous rendering area
             * @param newArea New rendering area
             */
            virtual void onResized(const Region2i &oldArea, const Region2i &newArea) = 0;
        };

        /**
         * An arbitrary rendering target for final image presentation, which
         * provides actual system or RHI target color object and a rendering area.
         *
         * In order to render multiple areas to single system or RHI target could be
         * used proxy, which will wrap actual objects.
         */
        class IRenderTarget {
        public:
            virtual ~IRenderTarget() = default;

            void addResizeListener(IRenderTargetResizeListener &listener);
            void removeResizeListener(IRenderTargetResizeListener &listener);

            EPixelFormat getTargetPixelFormat() const { return mPixelFormat; };

            Color4f getClearColor() const { return mClearColor; }
            float getDepthClearValue() const { return mDepthClearValue; }
            int32 getStencilClearValue() const { return mStencilClearValue; }

            float getAreaAspect() const;
            Size2i getTargetSize() const { return mTargetSize; }
            Size2i getAreaSize() const { return {mRenderingArea.getW(), mRenderingArea.getH()}; }

            Region2i getRenderingArea() const { return mRenderingArea; }

            virtual void extractDeclaration(class RHIGraphicsPipelineDesc &desc) const = 0;
            virtual void bind(RHIDrawList &drawList) const = 0;
            virtual bool isScreenTarget() const = 0;
            virtual bool isTextureTarget() const = 0;
            virtual bool isProxyTarget() const = 0;

        protected:
            EPixelFormat mPixelFormat = EPixelFormat::R8G8B8A8;
            Color4f mClearColor;
            float mDepthClearValue = 1.0f;
            int32 mStencilClearValue = 0;
            Size2i mTargetSize;
            Region2i mRenderingArea;
            TArray<IRenderTargetResizeListener *> mResizeListeners;
        };

    }
}

#endif //BERSERK_IRENDERTARGET_H
