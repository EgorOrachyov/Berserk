/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GRAPHICS_H
#define BERSERK_GRAPHICS_H

#include <Containers/TArray.h>
#include <GraphicsItems.h>
#include <RenderTargets/RenderTarget.h>

namespace Berserk {
    namespace Render {

        class Graphics {
        public:

            /**
             * Create graphics object with specified properties
             * @param size Size of the drawing area
             * @param background Background color used to clear graphics area
             */
            Graphics(const Size2i& size, const Color4f& background);
            ~Graphics();

            /**
             * Draw texture in the specified graphics point
             *
             * @param position Upper left image corner position on the graphics
             * @param texture Actual texture for bitmap data
             */
            void drawTexture(const Point2i& position, const TPtrShared<Texture2D> &texture);

            /**
             * Draw texture in the specified graphics point
             *
             * @param position Upper left image corner position on the graphics
             * @param texture Actual texture for bitmap data
             * @param area Area of the graphics where to fit image
             */
            void drawTexture(const Point2i& position, const TPtrShared<Texture2D> &texture, const Size2i& area);

            /**
             * Draw texture in the specified graphics point
             *
             * @param position Upper left image corner position on the graphics
             * @param texture Actual texture for bitmap data
             * @param area Area of the graphics where to fit image
             * @param region Region of the texture to be draw to the area
             */
            void drawTexture(const Point2i& position, const TPtrShared<Texture2D> &texture, const Size2i& area, const Region2i& region);

            /** Clear graphics content */
            void clear();

            /** Set color used as background when graphics is cleared */
            void setBackgroundColor(const Color4f& color);

            /** Set graphics area size in pixels */
            void setGraphicsSize(const Size2i& size);

            /** @return True if canvas dirty and must be updated */
            bool isDirty() const { return mIsDirty; }

            /** @return Graphics area size in pixels */
            const Size2i &getSize() const { return mSize; }

            /** @return Items in the graphics */
            const TArray<GraphicsTexture*> getTextureItems() const { return mTextureItems; }

            /** Max z-order depth */
            static const uint32 Z_FAR = -10000;

        private:

            friend class GraphicsRenderer;

            /** Compute single element alloc size, align to PLATFORM_ALIGN */
            static uint32 getElementSize();

            /** Return Z order and increments by step */
            int32 getElementZOrderAndIncrement();

            /** Delete all items from graphics */
            void deleteAllItems();

            /** Mark dirty, so renderer will refresh its cache */
            void markDirty();

            /** Mark clean by Graphics Renderer if renderer cache is sync to Graphics */
            void markClean();

            /** Single allocator, used to allocate all items */
            AllocPool mAllocPool;

            /** Items */
            TArray<GraphicsTexture*> mTextureItems;

            /** Target area of drawing */
            Size2i mSize;

            /** True if requires GPU update */
            bool mIsDirty = false;

            /** Order, set for the next drawn item */
            int32 mCurrentZOrder = Z_FAR;

            /** Step to increment order */
            int32 mZOrderStep = 1;

            /** Clear (background color of the graphics) color */
            Color4f mBackground;

        };


    }
}



#endif //BERSERK_GRAPHICS_H
