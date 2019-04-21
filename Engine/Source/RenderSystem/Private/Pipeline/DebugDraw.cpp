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
        ArrayList<DebugDrawManager::DrawRequest>& data = *mDebugDrawManager->getRenderQueue();
        IRenderDriver::ViewPort port = passInfo.getDisplayViewPort();

        Mat4x4f mat = Mat4x4f::orthographic(0, (float32)port.width, 0, (float32)port.height, -1, 1);

        mTextRender->use();
        mTextRender->setUniform("Texture0", 0);
        mTextRender->setUniform("Projection", mat);
        mDebugFont->getTexture()->bind(0u);

        passInfo.getFrameBuffer()->bindFrameBuffer();
        mRenderDriver->depthTest(false);
        mRenderDriver->viewPort(passInfo.getFrameBufferViewPort());
        mRenderDriver->blending(true, IRenderDriver::BLEND_FUNC_SRC_ALPHA, IRenderDriver::BLEND_FUNC_ONE_MINUS_SRC_ALPHA);

        for (uint32 i = 0; i < data.getSize(); i++)
        {
            if (data[i].mType == DebugDrawManager::eDRT_TEXT)
            {
                VertPTf vertex[TEXT_BUFFER_COUNT * 4];
                DebugDrawManager::DrawRequest& request = data[i];
                uint32 length = Strings<char, '\0'>::strlen(request.mStringBuffer);

                ArrayList<Resources::Character>& font = *mDebugFont->getCharacters();
                float32 xPos = request.mTextPosition.x;
                float32 yPos = request.mTextPosition.y;
                float32 z = request.mTextPosition.z;

                for (uint16 i = 0; i < length; i++)
                {
                    float32 x = xPos + (float32) font[request.mStringBuffer[i]].bearingX * request.mTextScale;
                    float32 y = yPos + ((float32) font[request.mStringBuffer[i]].bearingY - (float32) font[request.mStringBuffer[i]].height) * request.mTextScale;

                    float32 w = request.mTextScale * font[request.mStringBuffer[i]].width;
                    float32 h = request.mTextScale * font[request.mStringBuffer[i]].height;

                    float32 u = font[request.mStringBuffer[i]].texturePos.x;
                    float32 v = font[request.mStringBuffer[i]].texturePos.y;

                    float32 uw = font[request.mStringBuffer[i]].textureSize.x;
                    float32 vh = font[request.mStringBuffer[i]].textureSize.y;

                    vertex[i * 4 + 0].position = Vec3f(x, y, z);
                    vertex[i * 4 + 0].texcoords = Vec2f(u, v);

                    vertex[i * 4 + 1].position = Vec3f(x + w, y, z);
                    vertex[i * 4 + 1].texcoords = Vec2f(u + uw, v);

                    vertex[i * 4 + 2].position = Vec3f(x + w, y + h, z);
                    vertex[i * 4 + 2].texcoords = Vec2f(u + uw, v + vh);

                    vertex[i * 4 + 3].position = Vec3f(x, y + h, z);
                    vertex[i * 4 + 3].texcoords = Vec2f(u, v + vh);

                    xPos += (float32) font[request.mStringBuffer[i]].advanceX * request.mTextScale;
                }

                mTextBuffer->update(length * 4 * sizeof(VertPTf), vertex);
                mTextRender->setUniform("TextColor", request.mColor);
                mTextBuffer->draw(length * 6);
            }
        }

        mRenderDriver->blending(false);
    }

} // namespace Berserk::Render