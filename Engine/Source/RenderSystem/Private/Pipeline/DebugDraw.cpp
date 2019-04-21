//
// Created by Egor Orachyov on 19.04.2019.
//

#include "Pipeline/DebugDraw.h"
#include <Platform/VertexTypes.h>

namespace Berserk::Render
{

    DebugDraw::DebugDraw(const char *name, IAllocator *allocator) : IPipelineStage(name, allocator)
    {

        {
            mTextRender = mShaderManager->loadShader("{SHADERS}/DebugDraw/Text/meta-info.xml");
            mDebugFont = mFontManager->getDebugFont();
            mTextBuffer = mBufferManager->createGPUBuffer("DebugDrawTextBuffer");

            uint16 indices[TEXT_BUFFER_COUNT * 6];
            for (uint16 i = 0; i < TEXT_BUFFER_COUNT; i += 1)
            {
                indices[6 * i + 0] = 4 * i;
                indices[6 * i + 1] = 4 * i + 1;
                indices[6 * i + 2] = 4 * i + 2;
                indices[6 * i + 3] = 4 * i;
                indices[6 * i + 4] = 4 * i + 2;
                indices[6 * i + 5] = 4 * i + 3;
            }

            mTextBuffer->create(TEXT_BUFFER_COUNT * 4,
                                IGPUBuffer::eVT_VertexPT,
                                nullptr,
                                TEXT_BUFFER_COUNT * 6,
                                indices,
                                IRenderDriver::USAGE_STREAM_DRAW);
        }

        PUSH("Stage: DebugDrawText: initialized");
    }

    DebugDraw::~DebugDraw()
    {
        mShaderManager->deleteShader(mTextRender);
        mFontManager->deleteFont(mDebugFont);
        mBufferManager->deleteGPUBuffer(mTextBuffer);
        PUSH("Stage: DebugDrawText: de-initialized");
    }

    void DebugDraw::reload()
    {
        /* Nothing */
    }

    void DebugDraw::execute(RenderPassInfo &passInfo)
    {
        // Sort all the primitives to reduce shader and uniform change operations count
        ArrayList<DebugDrawManager::DrawRequest>& data = *mDebugDrawManager->getRenderQueue();
        data.sort();

        // Setup main engine frame buffer
        passInfo.getFrameBuffer()->bindFrameBuffer();
        mRenderDriver->viewPort(passInfo.getFrameBufferViewPort());

        for (uint32 current = 0; current < data.getSize(); current++)
        {
            if (data[current].mType == DebugDrawManager::eDRT_TEXT)
            {
                // Render in orthographic view space of the DISPLAY (not frame buffer)
                IRenderDriver::ViewPort port = passInfo.getDisplayViewPort();
                Mat4x4f orthographic = Mat4x4f::orthographic(0, (float32)port.width, 0, (float32)port.height, -1, 1);

                // Only for text
                mRenderDriver->blending(true, IRenderDriver::BLEND_FUNC_SRC_ALPHA, IRenderDriver::BLEND_FUNC_ONE_MINUS_SRC_ALPHA);

                mTextRender->use();
                mTextRender->setUniform("Texture0", 0);
                mTextRender->setUniform("Projection", orthographic);
                mDebugFont->getTexture()->bind(0u);

                uint32 i = current;

                while (i < data.getSize() && data[i].mType == DebugDrawManager::eDRT_TEXT)
                {
                    VertPTf vertex[TEXT_BUFFER_COUNT * 4];
                    DebugDrawManager::DrawRequest& request = data[i];
                    uint32 length = Strings<char, '\0'>::strlen(request.mStringBuffer);

                    ArrayList<Resources::Character>& font = *mDebugFont->getCharacters();
                    float32 xPos = request.mTextPosition.x;
                    float32 yPos = request.mTextPosition.y;
                    float32 z = request.mTextPosition.z;

                    for (uint16 j = 0; j < length; j++)
                    {
                        float32 x = xPos + (float32) font[request.mStringBuffer[j]].bearingX * request.mTextScale;
                        float32 y = yPos + ((float32) font[request.mStringBuffer[j]].bearingY - (float32) font[request.mStringBuffer[j]].height) * request.mTextScale;

                        float32 w = request.mTextScale * font[request.mStringBuffer[j]].width;
                        float32 h = request.mTextScale * font[request.mStringBuffer[j]].height;

                        float32 u = font[request.mStringBuffer[j]].texturePos.x;
                        float32 v = font[request.mStringBuffer[j]].texturePos.y;

                        float32 uw = font[request.mStringBuffer[j]].textureSize.x;
                        float32 vh = font[request.mStringBuffer[j]].textureSize.y;

                        vertex[j * 4 + 0].position  = Vec3f(x, y, z);
                        vertex[j * 4 + 0].texcoords = Vec2f(u, v);

                        vertex[j * 4 + 1].position  = Vec3f(x + w, y, z);
                        vertex[j * 4 + 1].texcoords = Vec2f(u + uw, v);

                        vertex[j * 4 + 2].position  = Vec3f(x + w, y + h, z);
                        vertex[j * 4 + 2].texcoords = Vec2f(u + uw, v + vh);

                        vertex[j * 4 + 3].position  = Vec3f(x, y + h, z);
                        vertex[j * 4 + 3].texcoords = Vec2f(u, v + vh);

                        xPos += (float32) font[request.mStringBuffer[j]].advanceX * request.mTextScale;
                    }

                    mRenderDriver->depthTest(data[i].mDepthTest);
                    mTextBuffer->update(length * 4 * sizeof(VertPTf), vertex);
                    mTextRender->setUniform("TextColor", request.mColor);
                    mTextBuffer->draw(length * 6);

                    i += 1;
                }

                // Disable blending (all text is done)
                mRenderDriver->blending(false);

                // Skip section with text
                current = i;

                // Need new range check for current
                continue;
            }
        }
    }

} // namespace Berserk::Render