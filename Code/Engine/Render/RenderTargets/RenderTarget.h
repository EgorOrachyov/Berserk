/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RENDERTARGET_H
#define BERSERK_RENDERTARGET_H

#include <Math/Region2i.h>
#include <Math/Point2i.h>
#include <TEnumMask.h>
#include <Containers/TArray.h>
#include <RHI/RHIDefinitions.h>

namespace Berserk {
    namespace Render {

        class RenderTargetResizeListener {
        public:
            virtual void onTargetResized(Size2i oldSize, Size2i newSize) = 0;
        };

        /**
         * @brief Rendering target
         *
         * Base class for any target, which can be used as a target buffer
         * (window or offscreen buffer) for rendering on GPU with RHI device.
         *
         * Implement this class interface for you target type to support the
         * same rendering techniques among several render targets.
         */
        class RenderTarget {
        public:

            virtual ~RenderTarget() = default;

            /** @return Size of area of this target */
            Size2i getAreaSize() const { return mAreaSize; }

            /** @return True if target has depth buffer */
            bool supportsDepthBuffer() const { return mHasDepth; }

            /** @return True if target has stencil buffer */
            bool supportsStencilBuffer() const { return mHasStencil; }

            /** @return True if target has */
            bool supportsColorBuffer() const { return mHasColor; }

            /** @return If this is offscreen target (as no underlying platform window) */
            bool isOffscreenTarget() const { return mOffscreenTarget; }

            /** Add resize listener */
            void addResizeListener(RenderTargetResizeListener& listener);

            /** Remove resize listener */
            void removeResizeListener(RenderTargetResizeListener& listener);

            /** Binds this render target to the draw list for drawing (viewport will be set automatically) */
            virtual void bind(class RHIDrawList& drawList) = 0;

            /** Binds this render target to the draw list for drawing (viewport will be set automatically) */
            virtual void bind(class RHIDrawList& drawList, const TEnumMask<EClearOption> &clearOptions) = 0;

            /** Bind target for the rendering with specified clear options and sub region of the drawing */
            virtual void bind(class RHIDrawList& drawList, const TEnumMask<EClearOption> &clearOptions, const Region2i& subRegion) = 0;

        protected:

            /** Notify listeners that target resized */
            void onResized(Size2i oldSize, Size2i newSize);

            Size2i mAreaSize;
            bool mOffscreenTarget = false;
            bool mHasDepth = false;
            bool mHasStencil = false;
            bool mHasColor = false;
            TArray<RenderTargetResizeListener*> mListeners;

        };

    }
}




#endif //BERSERK_RENDERTARGET_H
