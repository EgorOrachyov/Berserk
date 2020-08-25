/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Graphics.h>
#include <GraphicsItems.h>
#include <GraphicsRenderer.h>
#include <BuildOptions.h>

namespace Berserk {
    namespace Render {

        Graphics::Graphics(const Size2i &size, const Region2i &region, const TPtrShared <RenderTarget> &target)
            : mAllocPool(getElementSize()),
              mSize(size),
              mRegion(region),
              mTarget(target) {
            BERSERK_COND_ERROR_RET(target.isNotNull(), "Passed null render target");

            mRenderer = TPtrUnique<GraphicsRenderer>::make(*this);
        }

        Graphics::~Graphics() {
            clear();
        }

        void Graphics::drawLine(const GraphicsPen &pen, const Size2i& extent, uint32 width) {
            void* memory = mAllocPool.allocate(sizeof(GraphicsLine));
            auto item = new(memory) GraphicsLine();

            item->color = pen.getColor();
            item->position = pen.getPosition();
            item->zOrder = getElementZOrderAndIncrement();
            item->useAlpha = pen.isUsingAlpha();
            item->end = pen.getPosition() + extent;
            item->width = width;

            // If no alpha - explicitly set to max opacity
            if (!item->useAlpha) item->color.A() = 1.0f;
            if (item->useAlpha) mPrimitivesWithAlpha.add(item); else mPrimitives.add(item);

            markDirty();
        }
        
        void Graphics::drawRect(const GraphicsPen &pen, const Size2i &size, uint32 border) {
            void* memory = mAllocPool.allocate(sizeof(GraphicsRect));
            auto item = new(memory) GraphicsRect();

            item->color = pen.getColor();
            item->position = pen.getPosition();
            item->zOrder = getElementZOrderAndIncrement();
            item->size = size;
            item->border = border;
            item->useAlpha = pen.isUsingAlpha();

            // If no alpha - explicitly set to max opacity
            if (!item->useAlpha) item->color.A() = 1.0f;
            if (item->useAlpha) mPrimitivesWithAlpha.add(item); else mPrimitives.add(item);

            markDirty();
        }
        
        void Graphics::drawEllipse(const GraphicsPen &pen, const Size2i &radius, uint32 sections, uint32 border) {
            void* memory = mAllocPool.allocate(sizeof(GraphicsEllipse));
            auto item = new(memory) GraphicsEllipse();

            item->color = pen.getColor();
            item->zOrder = getElementZOrderAndIncrement();
            item->useAlpha = pen.isUsingAlpha();
            item->radius = radius;
            item->sections = Math::max(sections, GraphicsFilledEllipse::MIN_SECTIONS);;
            item->position = pen.getPosition() - radius;
            item->border = border;

            // If no alpha - explicitly set to max opacity
            if (!item->useAlpha) item->color.A() = 1.0f;
            if (item->useAlpha) mPrimitivesWithAlpha.add(item); else mPrimitives.add(item);

            markDirty();
        }

        void Graphics::drawCircle(const GraphicsPen &pen, uint32 radius, uint32 sections, uint32 border) {
            drawEllipse(pen, Size2i(radius,radius), sections, border);
        }
        
        void Graphics::drawFilledRect(const GraphicsPen &pen, const Size2i &size) {
            void* memory = mAllocPool.allocate(sizeof(GraphicsFilledRect));
            auto item = new(memory) GraphicsFilledRect();

            item->color = pen.getColor();
            item->position = pen.getPosition();
            item->zOrder = getElementZOrderAndIncrement();
            item->size = size;
            item->useAlpha = pen.isUsingAlpha();

            // If no alpha - explicitly set to max opacity
            if (!item->useAlpha) item->color.A() = 1.0f;
            if (item->useAlpha) mPrimitivesWithAlpha.add(item); else mPrimitives.add(item);

            markDirty();
        }

        void Graphics::drawFilledEllipse(const GraphicsPen &pen, const Size2i &radius, uint32 sections) {
            void* memory = mAllocPool.allocate(sizeof(GraphicsFilledEllipse));
            auto item = new(memory) GraphicsFilledEllipse();

            item->color = pen.getColor();
            item->zOrder = getElementZOrderAndIncrement();
            item->useAlpha = pen.isUsingAlpha();
            item->radius = radius;
            item->sections = Math::max(sections, GraphicsFilledEllipse::MIN_SECTIONS);;
            item->position = pen.getPosition() - radius;

            // If no alpha - explicitly set to max opacity
            if (!item->useAlpha) item->color.A() = 1.0f;
            if (item->useAlpha) mPrimitivesWithAlpha.add(item); else mPrimitives.add(item);

            markDirty();
        }
        
        void Graphics::drawFilledCircle(const GraphicsPen &pen, uint32 radius, uint32 sections) {
            drawFilledEllipse(pen, Size2i(radius,radius), sections);
        }

        
        void Graphics::drawTexture(const GraphicsPen &pen, const TPtrShared <Texture2D> &texture) {
            BERSERK_COND_ERROR_RET(texture.isNotNull(), "Passed null texture");
            drawTexture(pen, texture, Size2i(texture->getSize()));
        }
        
        void Graphics::drawTexture(const GraphicsPen &pen, const TPtrShared <Texture2D> &texture, const Size2i &area) {
            BERSERK_COND_ERROR_RET(texture.isNotNull(), "Passed null texture");
            drawTexture(pen, texture, area, Region2i(0, 0, texture->getSize()));
        }

        void Graphics::drawTexture(const GraphicsPen &pen, const TPtrShared <Texture2D> &texture, const Size2i &area, const Region2i &region) {
            BERSERK_COND_ERROR_RET(texture.isNotNull(), "Passed null texture");

            void* memory = mAllocPool.allocate(sizeof(GraphicsTexture));
            auto item = new (memory) GraphicsTexture();

            item->position = pen.getPosition();
            item->zOrder = getElementZOrderAndIncrement();
            item->texture = texture;
            item->areaSize = area;
            item->textureRect = region;
            item->isSRGB = texture->isInSRGB();
            item->useAlpha = false;
            item->color = pen.getColor();
            item->useTransparentColor = texture->isUsingTransparentColor();
            item->transparentColor = texture->getTransparentColor();

            mTextures.add(item);
            markDirty();
        }

        void Graphics::drawText(const GraphicsPen &pen, WString text, const TPtrShared<GpuFont> &font) {
            drawText(pen, std::move(text), font, 0);
        }
        
        void Graphics::drawText(const GraphicsPen &pen, WString text, const TPtrShared<GpuFont> &font, uint32 height) {
            BERSERK_COND_ERROR_RET(font.isNotNull(), "Passed null font");
            BERSERK_COND_ERROR_RET(font->isInitialized(), "Font resource is not initialized");

            void* memory = mAllocPool.allocate(sizeof(GraphicsText));
            auto item = new(memory) GraphicsText();

            item->position = pen.getPosition();
            item->color = pen.getColor();
            item->zOrder = getElementZOrderAndIncrement();
            item->useAlpha = false;
            item->font = font;
            item->text = std::move(text);
            item->height = height;

            // If no alpha - explicitly set to max opacity
            if (!item->useAlpha) item->color.A() = 1.0f;

            mTexts.add(item);
            markDirty();
        }

        void Graphics::clear() {
            markClean();
            deleteAllItems();
            mCurrentZOrder = Z_FAR;
            mRenderer->clearState();
        }

        void Graphics::draw(RHIDrawList &drawList) {
            mRenderer->draw(drawList);
            markClean();
        }

        void Graphics::fitAreaToTarget() {
            setGraphicsSize(mTarget->getAreaSize());
        }

        void Graphics::fitRegionToTarget() {
            setDrawRegion(Region2i(0,0,mTarget->getAreaSize()));
        }

        void Graphics::setBackgroundColor(const Color4f &color) {
            mBackground = color;
        }

        void Graphics::setGraphicsSize(const Size2i &size) {
            markDirty();
            mSize = size;
        }
        
        void Graphics::setDrawRegion(const Region2i &region) {
            mRegion = region;
        }

        uint32 Graphics::getElementSize() {
            uint64 sizes[] = {
                sizeof(GraphicsItem),
                sizeof(GraphicsTexture),
                sizeof(GraphicsPrimitive),
                sizeof(GraphicsFilledRect),
                sizeof(GraphicsFilledEllipse),
                sizeof(GraphicsLine),
                sizeof(GraphicsRect),
                sizeof(GraphicsEllipse),
                sizeof(GraphicsText)
            };

            // Compute maximum size
            uint64 size = sizes[0];
            for (auto s: sizes) {
                size = Math::max(s, size);
            }

            return Memory::alignAs(size, BERSERK_PLATFORM_ALIGN);
        }

        int32 Graphics::getElementZOrderAndIncrement() {
            int32 z = mCurrentZOrder;
            mCurrentZOrder += mZOrderStep;
            return z;
        }

        void Graphics::deleteAllItems() {
            for (auto item: mTexts) {
                item->~GraphicsText();
                mAllocPool.free(item);
            }

            for (auto item: mTextures) {
                item->~GraphicsTexture();
                mAllocPool.free(item);
            }

            for (auto item: mPrimitives) {
                item->~GraphicsPrimitive();
                mAllocPool.free(item);
            }

            for (auto item: mPrimitivesWithAlpha) {
                item->~GraphicsPrimitive();
                mAllocPool.free(item);
            }

            mTexts.clearNoDestructorCall();
            mTextures.clearNoDestructorCall();
            mPrimitives.clearNoDestructorCall();
            mPrimitivesWithAlpha.clearNoDestructorCall();
        }

        void Graphics::markDirty() {
            mIsDirty = true;
        }

        void Graphics::markClean() {
            mIsDirty = false;
        }


    }
}