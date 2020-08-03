/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <GraphicsRenderer.h>
#include <ShaderManager.h>
#include <Containers/TAlgo.h>

namespace Berserk {
    namespace Render {

        GraphicsRenderer::GraphicsRenderer(Graphics &graphics, const TPtrShared<RenderTarget> &target) {
            BERSERK_COND_ERROR_RET(target.isNotNull(), "Passed null target");
            mTarget = target;
            mGraphics = &graphics;
            mRegion = Region2i(0,0,mGraphics->getSize());

            initialize();
        }

        GraphicsRenderer::GraphicsRenderer(Graphics &graphics, const Region2i &region, const TPtrShared<RenderTarget> &target) {
            BERSERK_COND_ERROR_RET(target.isNotNull(), "Passed null target");
            mTarget = target;
            mGraphics = &graphics;
            mRegion = region;

            initialize();
        }

        void GraphicsRenderer::draw(RHIDrawList &drawList) {

        }

        void GraphicsRenderer::initialize() {
            auto& shaderManager = ShaderManager::getSingleton();

            {
                mTextures.shader = shaderManager.load("GraphicsTextureShader");
            }
        }

        void GraphicsRenderer::clearState() {
            {
                mTextures.uniforms.clear();
                mTextures.instanceData.clear();
                mTextures.vertexData.clear();
            }
        }

        void GraphicsRenderer::prepareData() {
            prepareTexturesData();
        }

        void GraphicsRenderer::prepareTexturesData() {
            if (mGraphics->getTextureItems().isEmpty()) {
                // Nothing to draw
                return;
            }

            mTextures.texturesSorted = mGraphics->getTextureItems();
            TAlgo::sort(mTextures.texturesSorted, [](const GraphicsTexture* a, const GraphicsTexture* b){ return a->zOrder < b->zOrder; });

            struct TextureVertData {
                float pos[3];
                float textCoords[2];

                TextureVertData(const Point2i& pos, const Size2i& area, const Region2i& textureSection, const Vec2f& scale) {

                }
            };
        }

    }
}