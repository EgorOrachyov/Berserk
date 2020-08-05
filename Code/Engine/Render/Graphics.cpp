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

        void Graphics::drawFilledRect(const GraphicsPen &pen, const Point2i &position, const Size2i &size) {
            void* memory = mAllocPool.allocate(sizeof(GraphicsPrimitiveRect));
            auto item = new(memory) GraphicsPrimitiveRect();

            item->color = pen.color;
            item->position = position;
            item->zOrder = getElementZOrderAndIncrement();
            item->size = size;

            mPrimitives.add(item);
            markDirty();
        }
        
        void Graphics::drawTexture(const Point2i &position,const TPtrShared<Texture2D> &texture) {
            BERSERK_COND_ERROR_RET(texture.isNotNull(), "Passed null texture");
            drawTexture(position,texture,Size2i(texture->getSize()));
        }
        
        void Graphics::drawTexture(const Point2i &position,const TPtrShared<Texture2D> &texture,const Size2i &area) {
            BERSERK_COND_ERROR_RET(texture.isNotNull(), "Passed null texture");
            drawTexture(position,texture,area,Region2i(0,0,texture->getSize()));
        }

        void Graphics::drawTexture(const Point2i &position,const TPtrShared<Texture2D> &texture,const Size2i &area, const Region2i &region) {
            BERSERK_COND_ERROR_RET(texture.isNotNull(), "Passed null texture");

            void* memory = mAllocPool.allocate(sizeof(GraphicsTexture));
            auto item = new (memory) GraphicsTexture();

            item->position = position;
            item->zOrder = getElementZOrderAndIncrement();
            item->texture = texture;
            item->areaSize = area;
            item->textureRect = region;
            item->isSRGB = texture->isInSRGB();
            item->useAlpha = texture->isUsingAlpha();
            item->useTransparentColor = texture->isUsingTransparentColor();
            item->transparentColor = texture->getTransparentColor();

            mTextures.add(item);
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
                sizeof(GraphicsPrimitiveRect)
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
            for (auto item: mTextures) {
                item->~GraphicsTexture();
                mAllocPool.free(item);
            }

            mTextures.clearNoDestructorCall();
        }

        void Graphics::markDirty() {
            mIsDirty = true;
        }

        void Graphics::markClean() {
            mIsDirty = false;
        }


    }
}