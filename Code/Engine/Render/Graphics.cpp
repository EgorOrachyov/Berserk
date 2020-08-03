/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Graphics.h>
#include <GraphicsItems.h>
#include <BuildOptions.h>

namespace Berserk {
    namespace Render {

        Graphics::Graphics(const Size2i &size, const Color4f &background)
            : mAllocPool(getElementSize()),
              mSize(size),
              mBackground(background) {

        }

        Graphics::~Graphics() {
            clear();
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
            item->color = Color4f(1.0f);
            item->zOrder = getElementZOrderAndIncrement();
            item->texture = texture;
            item->areaSize = area;
            item->textureRect = region;

            mTextureItems.add(item);
        }

        void Graphics::clear() {
            markDirty();
            deleteAllItems();
        }

        void Graphics::setBackgroundColor(const Berserk::Color4f &color) {
            mBackground = color;
        }

        void Graphics::setGraphicsSize(const Berserk::Size2i &size) {
            mSize = size;
        }

        uint32 Graphics::getElementSize() {
            uint64 sizes[] = {
                    sizeof(GraphicsItem),
                    sizeof(GraphicsTexture)
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
            for (auto item: mTextureItems) {
                item->~GraphicsTexture();
                mAllocPool.free(item);
            }

            mTextureItems.clearNoDestructorCall();
        }

        void Graphics::markDirty() {
            mIsDirty = true;
        }


    }
}