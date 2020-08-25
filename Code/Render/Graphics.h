/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GRAPHICS_H
#define BERSERK_GRAPHICS_H

#include <GraphicsPen.h>
#include <String/CString.h>
#include <String/WString.h>
#include <Containers/TArray.h>
#include <RenderResources/GpuFont.h>
#include <RenderResources/Texture2D.h>
#include <RenderTargets/RenderTarget.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief 2D Graphics
         *
         * Represents virtual 2D drawing area, allows to draw in canvas style
         * primitives onto graphics area and render on GPU side.
         *
         * Supports coloring, using alpha blending, and drawing textures with alpha.
         * Supported primitives: line, rect, ellipse, circle, 2D texture, text.
         */
        class Graphics {
        public:

            /**
             * Create graphics object with specified properties
             * @param size Size of the drawing area
             * @param region Area in target where to draw canvas
             * @param target GPU target for Graphics rendering
             */
            Graphics(const Size2i &size, const Region2i &region, const TPtrShared <RenderTarget> &target);
            ~Graphics();

            /**
             * Draw line with specified width
             * @param pen Pen used to for drawing settings
             * @param extent Line offset from start to the end
             * @param width Line width
             */
            void drawLine(const GraphicsPen& pen, const Size2i& extent, uint32 width);

            /**
             * Draw rectangle border lines
             * @param pen Pen used to for drawing settings
             * @param size Width and height of the rect
             * @param border Width of the line used to draw rectangle borders
             */
            void drawRect(const GraphicsPen& pen, const Size2i& size, uint32 border);

            /**
             * Draw ellipse border with specified center via pen, radius per x/y axes and border
             * @param pen Pen used to for drawing settings
             * @param radius Ellipse radius per x and y axis
             * @param sections Number of vertices in the ellipse to form area
             * @param border Width of the line used to draw ellipse borders
             */
            void drawEllipse(const GraphicsPen& pen, const Size2i& radius, uint32 sections, uint32 border);

            /**
             * Draw circle border with specified center via pen, radius  and border
             * @param pen Pen used to for drawing settings
             * @param center Circle center
             * @param radius Circle radius per x and y axis
             * @param sections Number of vertices in the circle to form area
             * @param border Width of the line used to draw circle borders
             */
            void drawCircle(const GraphicsPen& pen, uint32 radius, uint32 sections, uint32 border);

            /**
             * Draw filled rectangle with specified properties
             * @param pen Pen used to for drawing settings
             * @param size Width and height of the rect
             */
            void drawFilledRect(const GraphicsPen& pen, const Size2i& size);

            /**
             * Draw filled ellipse with specified center and radius per x/y axes
             * @param pen Pen used to for drawing settings
             * @param radius Ellipse radius per x and y axis
             * @param sections Number of vertices in the ellipse to form area
             */
            void drawFilledEllipse(const GraphicsPen& pen, const Size2i& radius, uint32 sections);

            /**
             * Draw filled circle with specified center and radius
             * @param pen Pen used to for drawing settings
             * @param radius Circle radius
             * @param sections Number of vertices in the circle to form area
             */
            void drawFilledCircle(const GraphicsPen& pen, uint32 radius, uint32 sections);

            /**
             * Draw texture in the specified graphics point
             * @param pen Pen used to for drawing settings
             * @param texture Actual texture for bitmap data
             */
            void drawTexture(const GraphicsPen &pen, const TPtrShared <Texture2D> &texture);

            /**
             * Draw texture in the specified graphics point
             * @param pen Pen used to for drawing settings
             * @param texture Actual texture for bitmap data
             * @param area Area of the graphics where to fit image
             */
            void drawTexture(const GraphicsPen &pen, const TPtrShared <Texture2D> &texture, const Size2i &area);

            /**
             * Draw texture in the specified graphics point
             * @param pen Pen used to for drawing settings
             * @param texture Actual texture for bitmap data
             * @param area Area of the graphics where to fit image
             * @param region Region of the texture to be draw to the area
             */
            void drawTexture(const GraphicsPen &pen, const TPtrShared <Texture2D> &texture, const Size2i &area, const Region2i &region);

            /**
             * Draw text string in the specified position with chosen font
             * @param pen Pen used to for drawing settings
             * @param text Text to draw
             * @param font Initialized font resource
             */
            void drawText(const GraphicsPen& pen, WString text, const TPtrShared<GpuFont> &font);

            /**
             * Draw text string in the specified position with chosen font
             * @param pen Pen used to for drawing settings
             * @param text Text to draw
             * @param font Initialized font resource
             * @param height Height in graphics units of the text
             */
            void drawText(const GraphicsPen& pen, WString text, const TPtrShared<GpuFont> &font, uint32 height);

            /** Clear graphics content */
            void clear();

            /** Draw graphics state with specified RHI draw list */
            void draw(RHIDrawList& drawList);

            /** Fit canvas size to target size */
            void fitAreaToTarget();

            /** Fit final draw area in target to target full size */
            void fitRegionToTarget();

            /** Set color used as background when graphics is cleared */
            void setBackgroundColor(const Color4f& color);

            /** Enable clear background with color */
            void setUseBackground(bool use) { mUseBackground = use; }

            /** Set graphics area size in pixels */
            void setGraphicsSize(const Size2i& size);

            /** Set area where to draw in target on GPU */
            void setDrawRegion(const Region2i& region);

            /** @return True if canvas dirty and must be updated */
            bool isDirty() const { return mIsDirty; }

            /** @return True if on clear fills background  */
            bool isUsingBackground() const { return mUseBackground; }

            /** @return Graphics area size in pixels */
            const Size2i &getSize() const { return mSize; }

            /** @return Texture items in the graphics */
            const TArray<class GraphicsTexture*> &getTextureItems() const { return mTextures; }

            /** @return Primitive items in the graphics */
            const TArray<class GraphicsPrimitive*> &getPrimitiveItems() const { return mPrimitives; }

            /** @return Primitive items in the graphics, which use alpha blending */
            const TArray<class GraphicsPrimitive*> &getPrimitiveItemsWithAlpha() const { return mPrimitivesWithAlpha; }

            /** @return Text items in the graphics */
            const TArray<class GraphicsText*> &getTextItems() const { return mTexts; }

            /** @return Area where to draw graphics on the GPU target */
            const Region2i& getRegion() const { return mRegion; }

            /** @return GPU target for rendering */
            const TPtrShared<RenderTarget> &getTarget() const { return mTarget; }

            /** Max z-order depth */
            static const int32 Z_FAR = -10000;

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

            /** Texture Items to be drawn */
            TArray<class GraphicsTexture*> mTextures;

            /** Primitives to be drawn */
            TArray<class GraphicsPrimitive*> mPrimitives;

            /** Primitives to be drawn with alpha blending */
            TArray<class GraphicsPrimitive*> mPrimitivesWithAlpha;

            /** Text requests to draw */
            TArray<class GraphicsText*> mTexts;

            /** GPU target */
            TPtrShared<RenderTarget> mTarget;

            /** Viewport of the target where to render */
            Region2i mRegion;

            /** Target area of drawing */
            Size2i mSize;

            /** True if requires GPU update */
            bool mIsDirty = false;

            /** True if clear background with color */
            bool mUseBackground = false;

            /** Order, set for the next drawn item */
            int32 mCurrentZOrder = Z_FAR;

            /** Step to increment order */
            int32 mZOrderStep = 1;

            /** Clear (background color of the graphics) color */
            Color4f mBackground;

            /** Encapsulates this graphics object rendering */
            TPtrUnique<class GraphicsRenderer> mRenderer;
        };


    }
}



#endif //BERSERK_GRAPHICS_H
